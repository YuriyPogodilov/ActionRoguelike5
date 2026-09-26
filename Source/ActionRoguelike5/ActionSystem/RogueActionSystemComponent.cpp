// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"

#include "GameplayTagContainer.h"

#include "RogueAction.h"
#include "RogueAttributeSet.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
	bWantsInitializeComponent = true;
}

void URogueActionSystemComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	if (Attributes == nullptr)
	{
		Attributes = NewObject<URogueAttributeSet>(this, URogueAttributeSet::StaticClass());
		UE_LOG(LogTemp, Warning, TEXT("No default AttributeSet defined. Set using SetDefaultAttributeSet()"
								"during Actor Construction or assign in Blueprint ActionComponent for %s"), *GetNameSafe(GetOwner()));
	}
	
	for (TFieldIterator<FStructProperty> PropIt(Attributes->GetClass()); PropIt; ++PropIt)
	{
		FRogueAttribute* FoundAttribute = PropIt->ContainerPtrToValuePtr<FRogueAttribute>(Attributes);
		
		FName AttributeTagName = FName("Attribute." + PropIt->GetName());
		FGameplayTag AttributeTag = FGameplayTag::RequestGameplayTag(AttributeTagName);
		
		CachedAttributes.Add(AttributeTag, FoundAttribute);
	}

	for (TSubclassOf<URogueAction> ActionClass : DefaultActions)
	{
		if (ensure(ActionClass))
		{
			GrantAction(ActionClass);
		}
	}
}

void URogueActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Attributes->InitializeAttribute();
}

void URogueActionSystemComponent::GrantAction(TSubclassOf<URogueAction> NewActionClass)
{
	URogueAction* NewAction = NewObject<URogueAction>(this, NewActionClass);
	Actions.Add(NewAction);
}

void URogueActionSystemComponent::StartAction(FGameplayTag InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action->GetActionName() == InActionName)
		{
			if (Action->CanStart())
			{
				Action->StartAction();
			}
			
			return;
		}
	}
	
	UE_LOGFMT(LogTemp, Warning, "No Action found with name {ActionName}", InActionName.ToString());
}

void URogueActionSystemComponent::StopAction(FGameplayTag InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action->GetActionName() == InActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction();
			}
			return;
		}
	}
	
	UE_LOGFMT(LogTemp, Warning, "No Action found with name {ActionName}", InActionName.ToString());
}

void URogueActionSystemComponent::SetDefaultAttributeSet(TSubclassOf<URogueAttributeSet> AttributeSetClass)
{
	check(!HasBeenInitialized());
	
	// Only available during construction of UObjects
	FObjectInitializer& ObjectInitializer = FObjectInitializer::Get();
	Attributes = Cast<URogueAttributeSet>(ObjectInitializer.CreateDefaultSubobject(this, "Attributes", AttributeSetClass, AttributeSetClass));
}

void URogueActionSystemComponent::ApplyAttributeChange(FGameplayTag AttributeTag, float InValueChange, EAttributeModifyType ModifyType)
{
	FRogueAttribute* FoundAttribute = GetAttribute(AttributeTag);
	check(FoundAttribute);
	
	float OldValue = FoundAttribute->GetValue();

	switch (ModifyType)
	{
	case Base:
		FoundAttribute->Base += InValueChange;
		break;
	case Modifier:
		FoundAttribute->Modifier += InValueChange;
		break;
	case OverrideBase:
		FoundAttribute->Base = InValueChange;
		break;
	default:
		check(false);
	}
	
	Attributes->PostAttributeChanged();
	
	// Native C++ listeners
	if (FOnAttributeChanged* Event = AttributeListeners.Find(AttributeTag))
	{
		Event->Broadcast(AttributeTag, FoundAttribute->GetValue(), OldValue);
	}
	
	// Blueprint listeners
	if (TArray<FOnAttributeChanged_Dynamic>* Events = AttributeDynamicListeners.Find(AttributeTag))
	{
		for (int i = Events->Num() - 1; i >= 0; --i)
		{
			FOnAttributeChanged_Dynamic& Event = (*Events)[i];
			bool bIsBound = Event.ExecuteIfBound(AttributeTag, FoundAttribute->GetValue(), OldValue);
			if (!bIsBound)
			{
				Events->RemoveAt(i);
				UE_LOG(LogTemp, Warning, TEXT("Cleaned up expired attribute delegate for %s"), *GetNameSafe(GetOwner()));
			}
		}
	}
	
	UE_LOGFMT(LogTemp, Log, "Changed Attribute: {0}, New: {1}, Old: {2}",
		AttributeTag.ToString(),
		FoundAttribute->GetValue(),
		OldValue
	);
}

FRogueAttribute* URogueActionSystemComponent::GetAttribute(FGameplayTag AttributeTag) const
{
	FRogueAttribute* const* FoundAttribute = CachedAttributes.Find(AttributeTag);
	
	return *FoundAttribute;
}

float URogueActionSystemComponent::GetAttributeValue(FGameplayTag AttributerTag) const
{
	FRogueAttribute* FoundAttribute = GetAttribute(AttributerTag);
	
	return FoundAttribute->GetValue();
}

FOnAttributeChanged& URogueActionSystemComponent::GetAttributeListener(FGameplayTag AttributeTag)
{
	return AttributeListeners.FindOrAdd(AttributeTag);
}

void URogueActionSystemComponent::AddDynamicAttributeListener(FOnAttributeChanged_Dynamic Event, FGameplayTag AttributeTag)
{
	TArray<FOnAttributeChanged_Dynamic>& Events = AttributeDynamicListeners.FindOrAdd(AttributeTag);
	Events.Add(Event);
}

void URogueActionSystemComponent::RemoveDynamicAttributeListener(FOnAttributeChanged_Dynamic Event)
{
	for (TPair<FGameplayTag, TArray<FOnAttributeChanged_Dynamic>>& Listener : AttributeDynamicListeners)
	{
		if (Listener.Value.RemoveSingle(Event) > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully removed blueprint binding."));
			break;
		}
	}
}
