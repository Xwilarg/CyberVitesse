//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#if WITH_EDITOR
#include "PopcornFXDetailsAttributeList.h"

#include "Assets/PopcornFXEffect.h"
#include "PopcornFXEmitter.h"
#include "PopcornFXEmitterComponent.h"
#include "Attributes/PopcornFXAttributeList.h"
#include "Editor/EditorHelpers.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyHandle.h"
#include "IPropertyUtilities.h"
#include "ScopedTransaction.h"

#include "SNumericEntryBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "PopcornFXDetailsAttributeList"
DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXDetailsAttributeList, Log, All);

//#define ATTRDEBUB_LOG		UE_LOG
#define ATTRDEBUB_LOG(...)	do { } while(0)

namespace
{
	UPopcornFXEffect		*ResolveEffect(const UPopcornFXAttributeList *attrList)
	{
		UPopcornFXEmitterComponent	*cpnt = Cast<UPopcornFXEmitterComponent>(attrList->GetOuter());
		if (cpnt != null)
			return cpnt->Effect;
		UPopcornFXEffect			*effect = Cast<UPopcornFXEffect>(attrList->GetOuter());
		return effect;
	}

	// Copy-Pasted form Editor/PropertyEditor/Private/SDetailSingleItemRow.cpp
	//
	class SMyConstrainedBox : public SCompoundWidget
	{
	public:
		SLATE_BEGIN_ARGS(SMyConstrainedBox)
			: _MinWidth(0)
			, _MaxWidth(0)
		{}
		SLATE_DEFAULT_SLOT(FArguments, Content)
			SLATE_ATTRIBUTE(float, MinWidth)
			SLATE_ATTRIBUTE(float, MaxWidth)
			SLATE_END_ARGS()

		void Construct(const FArguments& InArgs)
		{
			MinWidth = InArgs._MinWidth;
			MaxWidth = InArgs._MaxWidth;

			ChildSlot
				[
					InArgs._Content.Widget
				];
		}

		virtual FVector2D	ComputeDesiredSize(float LayoutScaleMultiplier) const override
		{
			const float MinWidthVal = MinWidth.Get();
			const float MaxWidthVal = MaxWidth.Get();

			if (MinWidthVal == 0.0f && MaxWidthVal == 0.0f)
				return SCompoundWidget::ComputeDesiredSize(LayoutScaleMultiplier);
			else
			{
				FVector2D ChildSize = ChildSlot.GetWidget()->GetDesiredSize();

				float XVal = FMath::Max(MinWidthVal, ChildSize.X);
				if (MaxWidthVal >= MinWidthVal)
				{
					XVal = FMath::Min(MaxWidthVal, XVal);
				}

				return FVector2D(XVal, ChildSize.Y);
			}
		}
	private:
		TAttribute<float> MinWidth;
		TAttribute<float> MaxWidth;
	};

	FString				GenerateTypeName(PopcornFX::EBaseTypeID typeId)
	{
		const PopcornFX::CBaseTypeTraits	&traits = PopcornFX::CBaseTypeTraits::Traits(typeId);
		FString			name;
		if (traits.IsFp)
			name = TEXT("F");
		else
			name = TEXT("I");
		int32		dim = traits.VectorDimension;
		name.AppendInt(dim);
		return name;
	}

	const FText				s_AxisTexts[2][4] = {
		// 0 : !isColor
			{
				LOCTEXT("X_Label", "X"),
				LOCTEXT("Y_Label", "Y"),
				LOCTEXT("Z_Label", "Z"),
				LOCTEXT("W_Label", "W"),
			},
			// 1: isColor
			{
				LOCTEXT("R_Label", "R"),
				LOCTEXT("G_Label", "G"),
				LOCTEXT("B_Label", "B"),
				LOCTEXT("A_Label", "A"),
			},
	};
	const FLinearColor		s_AxisColors[] = {
		FLinearColor::Red,
		FLinearColor::Green,
		FLinearColor::Blue,
		FLinearColor::Black,
	};

	template <typename _Scalar>
	struct IsFloat
	{
		enum { Value = false };
	};

	template <>
	struct IsFloat<float>
	{
		enum { Value = true };
	};


	class	SAttributeDesc : public SCompoundWidget
	{
	public:
		typedef SAttributeDesc						TSelf;
		typedef FPopcornFXDetailsAttributeList		TParent;

		SLATE_BEGIN_ARGS(SAttributeDesc)
		{}
			SLATE_ARGUMENT(int32, AttribI)
			SLATE_ARGUMENT(TWeakPtr<FPopcornFXDetailsAttributeList>, Parent)
			SLATE_ARGUMENT(TOptional<bool>, Dirtify)
			SLATE_ARGUMENT(TOptional<bool>, ReadOnly)
			SLATE_END_ARGS()
		SAttributeDesc()
		{
			ATTRDEBUB_LOG(LogPopcornFXDetailsAttributeList, Log, TEXT("SAttributeDesc ctor %p"), this);
		}

		~SAttributeDesc()
		{
			ATTRDEBUB_LOG(LogPopcornFXDetailsAttributeList, Log, TEXT("SAttributeDesc dotr %p"), this);
		}

		void Construct(const FArguments& InArgs)
		{
			m_Parent = InArgs._Parent;
			m_Index = uint32(InArgs._AttribI);

			m_Dirtify = InArgs._Dirtify.Get(true);
			m_ReadOnly = InArgs._ReadOnly.Get(false);

			if (!Init())
				return;

			const FVector2D Icon32(32.0f, 32.0f);

			TSharedPtr<SVerticalBox>	vbox;

			SAssignNew(vbox, SVerticalBox);

			vbox->AddSlot()
				.Padding(1.f)
				.AutoHeight()
				.HAlign(HAlign_Fill)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
						.Padding(1.f)
						.AutoWidth()
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.Text(m_Title)
							.ToolTipText(m_Description)
						]
					+ SHorizontalBox::Slot()
						.Padding(1.f)
						.FillWidth(1.f)
						.HAlign(HAlign_Right)
						[
							SNew(STextBlock)
							.Text(m_ShortDescription)
							.ToolTipText(m_Description)
						]
					+ SHorizontalBox::Slot()
						.Padding(1.f)
						.AutoWidth()
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							MakeResetButton()
						]
				];


			for (hh_u32 dimi = 0; dimi < m_Traits->VectorDimension; ++dimi)
			{
				vbox->AddSlot()
					.AutoHeight()
					.HAlign(HAlign_Fill)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(1.f)
						.FillWidth(1.f)
						.HAlign(HAlign_Fill)
						[
							SNew(SMyConstrainedBox)
							.MinWidth(125.f * 3.f)
							.MaxWidth(125.f * 3.f)
							[
								MakeAxis(dimi)
							]
						]
						+ SHorizontalBox::Slot()
							.Padding(1.f)
							.AutoWidth()
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								MakeResetButton(dimi)
							]
					];
			}

			TSharedPtr<SHorizontalBox>	hbox;
			SAssignNew(hbox, SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush(m_AttributeIcon))
				]
			+ SHorizontalBox::Slot()
				.FillWidth(1.f)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					vbox.ToSharedRef()
				];

			ChildSlot
				[
					hbox.ToSharedRef()
				];
		}

	private:
		bool		Init()
		{
			TSharedPtr<TParent>		parent = m_Parent.Pin();
			if (!parent.IsValid())
				return false;

			//m_AttributesPty = parent->m_SamplersPty;
			//if (!m_AttributesPty->IsValidHandle())
			//	return false;

			//m_AttributesRawDataPty = parent->m_AttributesRawDataPty;

			const UPopcornFXAttributeList		*attrList = parent->AttrList();
			check(attrList != null);

			//m_Parent = parent;
			//m_Index = index;
			UPopcornFXEffect			*effect = ResolveEffect(attrList);
			if (effect == null)
				return false;
			const PopcornFX::CParticleAttributeDeclaration		*decl = attrList->GetAttributeDeclaration(effect, m_Index);
			if (decl == null)
				return false;
			const FPopcornFXAttributeDesc	*desc = attrList->GetAttributeDesc(m_Index);
			check(desc != null);

			m_Traits = &(PopcornFX::CBaseTypeTraits::Traits((PopcornFX::EBaseTypeID)desc->AttributeBaseTypeID()));

			const FString			name = desc->AttributeName();

			{
				FString			title;
				title = "[";
				title.AppendInt(int32(m_Index));
				title += "] ";
				title += name;
				m_Title = FText::FromString(title);
			}
			{
				FString			description;
				FString			shortDescription;
				description = decl->AttributeDescription().Data();
				description = description.Replace(TEXT("\\n"), TEXT("\n"));
				int32		shortOffset;
				if (description.FindChar('\n', shortOffset))
					shortDescription = description.Left(shortOffset - 1);
				else
					shortDescription = description;
				m_Description = FText::FromString(description);
				m_ShortDescription = FText::FromString(shortDescription);
			}

			const FString			typeName = GenerateTypeName((PopcornFX::EBaseTypeID)desc->AttributeBaseTypeID());
			m_AttributeIcon = *(TEXT("PopcornFX.Attribute.") + typeName);

			m_IsColor =
				m_Traits->IsFp && m_Traits->VectorDimension == 4 &&
				(name.Contains(TEXT("color")) || name.Contains(TEXT("colour")) || name.Contains(TEXT("rgb")));
			m_HasMin = decl->HasMin();
			m_HasMax = decl->HasMax();
			//m_HasMin = false;
			//m_HasMax = false;

			m_Min = decl->GetMinValue();
			m_Max = decl->GetMaxValue();
			m_Def = decl->GetDefaultValue();

			return true;
		}

		TSharedRef<SWidget>		MakeAxis(uint32 dimi)
		{
			check(dimi < m_Traits->VectorDimension);
			if (m_Traits->IsFp)
				return _MakeAxis<float>(dimi);
			return _MakeAxis<int32>(dimi);
		}

		template <typename _Scalar>
		TSharedRef<SWidget>		_MakeAxis(uint32 dimi)
		{
			// Access a shared reference to 'this'
			TSharedRef<TSelf> sharedThis = SharedThis(this);

			check(dimi < m_Traits->VectorDimension);

			const bool		isColor = m_IsColor;

			TSharedPtr< SNumericEntryBox<_Scalar> >	axis;
			if (m_ReadOnly)
			{
				SAssignNew(axis, SNumericEntryBox<_Scalar>)
					.AllowSpin(true)

					.MinValue(m_HasMin ? TOptional<_Scalar>(m_Min.Get<_Scalar>()[dimi]) : TOptional<_Scalar>())
					.MaxValue(m_HasMax ? TOptional<_Scalar>(m_Max.Get<_Scalar>()[dimi]) : TOptional<_Scalar>())
					.MinSliderValue(m_HasMin ? TOptional<_Scalar>(m_Min.Get<_Scalar>()[dimi]) : TOptional<_Scalar>())
					.MaxSliderValue(m_HasMax ? TOptional<_Scalar>(m_Max.Get<_Scalar>()[dimi]) : TOptional<_Scalar>())

					.Value(sharedThis, &TSelf::GetValue<_Scalar>, dimi)

					.LabelVAlign(VAlign_Center)
					.LabelPadding(0)
					.Label()
					[
						SNumericEntryBox<float>::BuildLabel(s_AxisTexts[isColor][dimi], FLinearColor::White, s_AxisColors[dimi])
					];
			}
			else
			{
				SAssignNew(axis, SNumericEntryBox<_Scalar>)
					.AllowSpin(true)

					.MinValue(m_HasMin ? TOptional<_Scalar>(m_Min.Get<_Scalar>()[dimi]) : TOptional<_Scalar>())
					.MaxValue(m_HasMax ? TOptional<_Scalar>(m_Max.Get<_Scalar>()[dimi]) : TOptional<_Scalar>())
					.MinSliderValue(m_HasMin ? TOptional<_Scalar>(m_Min.Get<_Scalar>()[dimi]) : TOptional<_Scalar>())
					.MaxSliderValue(m_HasMax ? TOptional<_Scalar>(m_Max.Get<_Scalar>()[dimi]) : TOptional<_Scalar>())

					.Value(sharedThis, &TSelf::GetValue<_Scalar>, dimi)
					.OnValueChanged(sharedThis, &TSelf::OnValueChanged<_Scalar>, dimi)
					.OnValueCommitted(sharedThis, &TSelf::OnValueCommitted<_Scalar>, dimi)

					.LabelVAlign(VAlign_Center)
					.LabelPadding(0)
					.Label()
					[
						SNumericEntryBox<float>::BuildLabel(s_AxisTexts[isColor][dimi], FLinearColor::White, s_AxisColors[dimi])
					];
			}
			return axis.ToSharedRef();
		}

		TSharedRef<SWidget>		MakeResetButton(uint32 dimi)
		{
			return SNew(SButton)
				.OnClicked(this, &TSelf::OnDimResetClicked, dimi)
				.Visibility(this, &TSelf::GetDimResetVisibility, dimi)
				.ToolTipText(LOCTEXT("ResetToDefaultToolTip", "Reset to Default"))
				.ButtonStyle(FEditorStyle::Get(), "NoBorder")
				.Content()
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("PropertyWindow.DiffersFromDefault"))
				];
		}

		TSharedRef<SWidget>		MakeResetButton()
		{
			return SNew(SButton)
				.OnClicked(this, &TSelf::OnResetClicked)
				.Visibility(this, &TSelf::GetResetVisibility)
				.ToolTipText(LOCTEXT("ResetToDefaultToolTip", "Reset to Default"))
				.ButtonStyle(FEditorStyle::Get(), "NoBorder")
				.Content()
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("PropertyWindow.DiffersFromDefault"))
				];
		}

		bool			_GetAttrib(UPopcornFXAttributeList *&outAttribList, PopcornFX::SAttributesContainer *&outContainerIFP) const
		{
			TSharedPtr<TParent>		parent = m_Parent.Pin();
			if (!parent.IsValid())
				return false;
			outAttribList = parent->AttrList();
			if (outAttribList == null)
				return false;
			outContainerIFP = *outAttribList->AttributesContainerIFP();
			return true;
		}

		template <typename _Scalar>
		TOptional<_Scalar>		GetValue(uint32 dimi) const
		{
			UPopcornFXAttributeList				*attrList;
			PopcornFX::SAttributesContainer		*containerifp;
			if (!_GetAttrib(attrList, containerifp))
				return TOptional<_Scalar>();
			return attrList->AttributeRawDataAttributes()[m_Index].Get<_Scalar>()[dimi];
		}

		template <typename _Scalar>
		void		OnValueChanged(const _Scalar value, uint32 dimi)
		{
			if (m_ReadOnly)
				return;
			UPopcornFXAttributeList				*attrList;
			PopcornFX::SAttributesContainer		*containerifp;
			if (!_GetAttrib(attrList, containerifp))
				return;
			//attrList->SetFlags(RF_Transactional);
			//attrList->Modify();
			attrList->AttributeRawDataAttributes()[m_Index].Get<_Scalar>()[dimi] = value;
			if (containerifp != null)
				containerifp->Attributes()[m_Index].Get<_Scalar>()[dimi] = value;
		}

		template <typename _Scalar>
		void		OnValueCommitted(const _Scalar value, ETextCommit::Type CommitInfo, uint32 dimi)
		{
			if (m_ReadOnly)
				return;
			UPopcornFXAttributeList				*attrList;
			PopcornFX::SAttributesContainer		*containerifp;
			if (!_GetAttrib(attrList, containerifp))
				return;

			attrList->SetFlags(RF_Transactional);

			const FScopedTransaction Transaction(LOCTEXT("AttributeCommit", "Attribute Value Commit"));

			//UE_LOG(LogPopcornFXDetailsAttributeList, Log, TEXT("--- DETAIL ATTRLIST commit %p %f --- %s"), attrList, float(value), *attrList->GetFullName());

			attrList->Modify();

			attrList->AttributeRawDataAttributes()[m_Index].Get<_Scalar>()[dimi] = value;
			if (containerifp != null)
				containerifp->Attributes()[m_Index].Get<_Scalar>()[dimi] = value;

			attrList->PostEditChange();
		}

		FReply				OnResetClicked()
		{
			if (m_ReadOnly)
				return FReply::Handled();
			UPopcornFXAttributeList				*attrList;
			PopcornFX::SAttributesContainer		*containerifp;
			if (!_GetAttrib(attrList, containerifp))
				return FReply::Handled();

			const FScopedTransaction Transaction(LOCTEXT("AttributeReset", "Attribute Reset"));
			attrList->SetFlags(RF_Transactional);
			attrList->Modify();

			const PopcornFX::SAttributesContainer::SAttrib	&value = m_Def;

			attrList->AttributeRawDataAttributes()[m_Index] = value;
			if (containerifp != null)
				containerifp->Attributes()[m_Index] = value;

			attrList->PostEditChange();
			return FReply::Handled();
		}

		FReply				OnDimResetClicked(uint32 dimi)
		{
			if (m_ReadOnly)
				return FReply::Handled();
			UPopcornFXAttributeList				*attrList;
			PopcornFX::SAttributesContainer		*containerifp;
			if (!_GetAttrib(attrList, containerifp))
				return FReply::Handled();

			const FScopedTransaction Transaction(LOCTEXT("AttributeResetDim", "Attribute Reset Dimension"));
			attrList->SetFlags(RF_Transactional);
			attrList->Modify();

			const uint32	value = m_Def.Get<uint32>()[dimi];

			attrList->AttributeRawDataAttributes()[m_Index].Get<uint32>()[dimi] = value;
			if (containerifp != null)
				containerifp->Attributes()[m_Index].Get<uint32>()[dimi] = value;

			attrList->PostEditChange();
			return FReply::Handled();
		}

		EVisibility			GetResetVisibility() const
		{
			UPopcornFXAttributeList				*attrList;
			PopcornFX::SAttributesContainer		*containerifp;
			if (!_GetAttrib(attrList, containerifp))
				return EVisibility::Hidden;
			PopcornFX::SAttributesContainer::SAttrib	attribValue = attrList->AttributeRawDataAttributes()[m_Index];
			for (uint32 dimi = 0; dimi < m_Traits->VectorDimension; ++dimi)
			{
				if (attribValue.Get<uint32>()[dimi] != m_Def.Get<uint32>()[dimi])
					return EVisibility::Visible;
			}
			return EVisibility::Hidden;
		}

		EVisibility			GetDimResetVisibility(uint32 dimi) const
		{
			UPopcornFXAttributeList				*attrList;
			PopcornFX::SAttributesContainer		*containerifp;
			if (!_GetAttrib(attrList, containerifp))
				return EVisibility::Hidden;
			PopcornFX::SAttributesContainer::SAttrib	attribValue = attrList->AttributeRawDataAttributes()[m_Index];
			if (attribValue.Get<uint32>()[dimi] != m_Def.Get<uint32>()[dimi])
				return EVisibility::Visible;
			return EVisibility::Hidden;
		}

	private:
		TWeakPtr<TParent>	m_Parent;

		//TSharedPtr<IPropertyHandle>		m_AttributesPty;
		//TSharedPtr<IPropertyHandle>	m_AttributesRawDataPty;

		bool				m_Dirtify = true;
		bool				m_ReadOnly = false;

		uint32				m_Index;
		const PopcornFX::CBaseTypeTraits	*m_Traits;

		FText				m_Title;
		FText				m_Description;
		FText				m_ShortDescription;
		FName				m_AttributeIcon;

		bool				m_IsColor;
		bool				m_HasMin;
		bool				m_HasMax;

		PopcornFX::SAttributesContainer::SAttrib	m_Min;
		PopcornFX::SAttributesContainer::SAttrib	m_Max;
		PopcornFX::SAttributesContainer::SAttrib	m_Def;
	};


	class	SAttributeSamplerDesc : public SCompoundWidget
	{
	public:
		typedef SAttributeSamplerDesc						TSelf;
		typedef FPopcornFXDetailsAttributeList		TParent;

		SLATE_BEGIN_ARGS(SAttributeSamplerDesc)
		{}
			SLATE_ARGUMENT(int32, SamplerI)
			SLATE_ARGUMENT(TWeakPtr<FPopcornFXDetailsAttributeList>, Parent)
			SLATE_ARGUMENT(TOptional<bool>, Dirtify)
			SLATE_ARGUMENT(TOptional<bool>, ReadOnly)
			SLATE_END_ARGS()
			SAttributeSamplerDesc()
		{
			ATTRDEBUB_LOG(LogPopcornFXDetailsAttributeList, Log, TEXT("SAttributeSamplerDesc ctor %p"), this);
		}

		~SAttributeSamplerDesc()
		{
			ATTRDEBUB_LOG(LogPopcornFXDetailsAttributeList, Log, TEXT("SAttributeSamplerDesc dotr %p"), this);
		}

		void Construct(const FArguments& InArgs)
		{
			m_Parent = InArgs._Parent;
			m_Index = uint32(InArgs._SamplerI);

			m_Dirtify = InArgs._Dirtify.Get(true);
			m_ReadOnly = InArgs._ReadOnly.Get(false);

			if (!Init())
				return;

			uint32	childCount = 0;
			const FVector2D Icon32(32.0f, 32.0f);

			m_SamplersPty->GetNumChildren(childCount);
			if (m_Index >= childCount)
				return;

			TSharedPtr<IPropertyHandle>		samplerPty = m_SamplersPty->GetChildHandle(m_Index);
			HH_ASSERT(samplerPty.IsValid() && samplerPty->IsValidHandle());

			TSharedPtr<IPropertyHandle>		samplerActorPty = samplerPty->GetChildHandle(GET_MEMBER_NAME_STRING_CHECKED(FPopcornFXSamplerDesc, m_AttributeSamplerActor));
			HH_ASSERT(samplerActorPty.IsValid() && samplerActorPty->IsValidHandle());

			TSharedPtr<IPropertyHandle>		samplerCpntPty = samplerPty->GetChildHandle(GET_MEMBER_NAME_STRING_CHECKED(FPopcornFXSamplerDesc, m_AttributeSamplerComponentProperty));
			HH_ASSERT(samplerCpntPty.IsValid() && samplerCpntPty->IsValidHandle());

			TSharedPtr<SVerticalBox>	vbox;

			SAssignNew(vbox, SVerticalBox);

			vbox->AddSlot()
				.Padding(1.f)
				.AutoHeight()
				.HAlign(HAlign_Fill)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(1.f)
					.AutoWidth()
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Text(m_Title)
						.ToolTipText(m_Description)
					]
					+ SHorizontalBox::Slot()
						.Padding(1.f)
						.FillWidth(1.f)
						.HAlign(HAlign_Right)
						[
							SNew(STextBlock)
							.Text(m_ShortDescription)
							.ToolTipText(m_Description)
						]
#if 0
					+ SHorizontalBox::Slot()
						.Padding(1.f)
						.AutoWidth()
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							MakeResetButton()
						]
#endif
				];


			vbox->AddSlot()
				.Padding(1.f)
				.AutoHeight()
				.HAlign(HAlign_Fill)
				[
					samplerActorPty->CreatePropertyValueWidget()
				];
			vbox->AddSlot()
				.Padding(1.f)
				.AutoHeight()
				.HAlign(HAlign_Fill)
				[
					samplerCpntPty->CreatePropertyValueWidget()
				];

			TSharedPtr<SVerticalBox>	iconsVbox;

			SAssignNew(iconsVbox, SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.VAlign(VAlign_Top)
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush(m_SamplerIcon))
			];

			if (!m_HasDefaultDescriptor)
			{
				static const FName	badIcon = "PopcornFX.BadIcon32";

				iconsVbox->AddSlot()
				.AutoHeight()
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush(badIcon))
				];
			}

			TSharedPtr<SHorizontalBox>	hbox;
			SAssignNew(hbox, SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					iconsVbox.ToSharedRef()
				]
			+ SHorizontalBox::Slot()
				.FillWidth(1.f)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					vbox.ToSharedRef()
				];

			ChildSlot
				[
					hbox.ToSharedRef()
				];
		}

	private:
		bool		Init()
		{
			TSharedPtr<TParent>					parent = m_Parent.Pin();
			if (!parent.IsValid())
				return false;

			m_SamplersPty = parent->m_SamplersPty;
			if (!m_SamplersPty->IsValidHandle())
				return false;

			const UPopcornFXAttributeList		*attrList = parent->AttrList();
			check(attrList != null);

			//m_Parent = parent;
			//m_Index = index;
			const FPopcornFXSamplerDesc			*desc = attrList->GetSamplerDesc(m_Index);
			if (desc == null)
				return false;


			m_SamplerType = desc->m_SamplerType;

			const FString			name = desc->SamplerName();

			{
				FString			title;
				title = "[";
				title.AppendInt(int32(m_Index));
				title += "] ";
				title += name;
				m_Title = FText::FromString(title);
			}

			FString			defNode;

			UPopcornFXEffect			*effect = ResolveEffect(attrList);
			if (effect == null)
				return false;
			const PopcornFX::CParticleSampler		*particleSampler = attrList->GetParticleSampler(effect, m_Index);
			if (particleSampler == null)
				return true;
			m_HasDefaultDescriptor = particleSampler->GetSamplerDefaultDescriptor() != null;
			const char			*nodeName = ResolveAttribSamplerNodeName(particleSampler, m_SamplerType);
			if (nodeName != null)
			{
				defNode = nodeName;
				m_SamplerIcon = FName(*("PopcornFX.Node." + defNode));
			}
			else
			{
				defNode = "?????";
				m_SamplerIcon = FName(TEXT("PopcornFX.BadIcon32"));
			}

			m_Description = FText::FromString(name + " (default: " + defNode + ")");
			m_ShortDescription = FText::FromString(defNode);
			return true;
		}

	private:
		TWeakPtr<TParent>	m_Parent;

		//TSharedPtr<IPropertyHandle>	m_AttributesPty;
		TSharedPtr<IPropertyHandle>		m_SamplersPty;

		bool				m_Dirtify = true;
		bool				m_ReadOnly = false;
		bool				m_HasDefaultDescriptor = false;

		uint32				m_Index;

		EPopcornFXAttributeSamplerType::Type	m_SamplerType;

		FText				m_Title;
		FText				m_Description;
		FText				m_ShortDescription;
		FName				m_SamplerIcon;
	};
}

//static
TSharedRef<IDetailCustomization>		FPopcornFXDetailsAttributeList::MakeInstance()
{
	return MakeShareable(new FPopcornFXDetailsAttributeList);
}

FPopcornFXDetailsAttributeList::FPopcornFXDetailsAttributeList()
	: m_FileVersionId(0)
{
	ATTRDEBUB_LOG(LogPopcornFXDetailsAttributeList, Log, TEXT("FPopcornFXDetailsAttributeList ctor %p"), this);
}

FPopcornFXDetailsAttributeList::~FPopcornFXDetailsAttributeList()
{
	ATTRDEBUB_LOG(LogPopcornFXDetailsAttributeList, Log, TEXT("FPopcornFXDetailsAttributeList dtor %p"), this);
}

UPopcornFXAttributeList				*FPopcornFXDetailsAttributeList::UnsafeAttrList()
{
	const TArray<TWeakObjectPtr<UObject> >	&objects = m_BeingCustomized;
	//m_DetailBuilder->GetObjectsBeingCustomized(objects);
	if (objects.Num() != 1)
		return null;
	UObject							*outer = objects[0].Get();
	UPopcornFXAttributeList			*attrList = null;
	APopcornFXEmitter				*emitter = null;
	UPopcornFXEmitterComponent		*emitterComponent = null;
	UPopcornFXEffect				*effect = null;

	if ((attrList = Cast<UPopcornFXAttributeList>(outer)) != null)
		outer = attrList->GetOuter();
	else if ((emitter = Cast<APopcornFXEmitter>(outer)) != null)
	{
		outer = emitter->PopcornFXEmitterComponent;
	}
	else
		outer = objects[0].Get();

	// try re-fetch attribute list from Outer to make sur everything is up to date

	if ((emitterComponent = Cast<UPopcornFXEmitterComponent>(outer)) != null)
	{
		// Don't try to display attributes on an emitter with no effect attached
		if (emitterComponent->Effect == null)
			return null;
		UPopcornFXAttributeList		*a = emitterComponent->GetAttributeList();
		if (!HH_VERIFY(a != null))
			return null;
		HH_ASSERT(attrList == null || attrList == a);
		attrList = a;
	}
	else if ((effect = Cast<UPopcornFXEffect>(outer)) != null)
	{
		UPopcornFXAttributeList		*a = effect->GetDefaultAttributeList();
		if (!HH_VERIFY(a != null))
			return null;
		HH_ASSERT(attrList == null || attrList == a);
		attrList = a;
	}
	else
	{
		HH_ASSERT_NOT_REACHED();
	}
	return attrList;
}

void	FPopcornFXDetailsAttributeList::RebuildAndRefresh()
{
	RebuildAttributes();
	RebuildSamplers();
	if (HH_VERIFY(m_PropertyUtilities.IsValid()))
		m_PropertyUtilities->RequestRefresh();
}

UPopcornFXAttributeList		*FPopcornFXDetailsAttributeList::AttrList()
{
	UPopcornFXAttributeList		*attrList = UnsafeAttrList();
	if (attrList == null)
	{
		// We need to clear the remaining attributes
		if ((m_SAttributes->NumSlots() > 0 ||
			m_SSamplers->NumSlots() > 0) &&
			HH_VERIFY(m_PropertyUtilities.IsValid()))
			m_PropertyUtilities->EnqueueDeferredAction(FSimpleDelegate::CreateSP(this, &FPopcornFXDetailsAttributeList::RebuildAndRefresh));
		return null;
	}
	if (m_FileVersionId != attrList->FileVersionId())
	{
		if (HH_VERIFY(m_PropertyUtilities.IsValid()))
			m_PropertyUtilities->EnqueueDeferredAction(FSimpleDelegate::CreateSP(this, &FPopcornFXDetailsAttributeList::RebuildIFN));
		return null;
	}
	return attrList;
}

void	FPopcornFXDetailsAttributeList::RebuildIFN()
{
	const UPopcornFXAttributeList		*attrList = UnsafeAttrList(); // AttrList() will ask for rebuild ifn, we dont want that here
	if (!HH_VERIFY(attrList != null))
		return;
	if (m_FileVersionId != attrList->FileVersionId())
		Rebuild();
}

void	FPopcornFXDetailsAttributeList::Rebuild()
{
	const UPopcornFXAttributeList		*attrList = UnsafeAttrList(); // AttrList() will ask for rebuild ifn, we dont want that here
	if (attrList == null)
		return;
	m_FileVersionId = attrList->FileVersionId();
	RebuildAndRefresh();
}

void	FPopcornFXDetailsAttributeList::RebuildAttributes()
{
	ATTRDEBUB_LOG(LogPopcornFXDetailsAttributeList, Log, TEXT("rebuild attributes"));
	m_SAttributes->ClearChildren();

	const UPopcornFXAttributeList		*attrList = AttrList();
	if (attrList == null)
		return;

	const uint32			attrCount = attrList->AttributeCount();
	
	for (hh_u32 attri = 0; attri < attrCount; ++attri)
	{
		TSharedPtr<SAttributeDesc>		sdesc;
		m_SAttributes->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(sdesc, SAttributeDesc)
				.Parent(SharedThis(this))
				.AttribI(attri)
			]
		;
		sdesc = null;
	}
}

void	FPopcornFXDetailsAttributeList::RebuildSamplers()
{
	ATTRDEBUB_LOG(LogPopcornFXDetailsAttributeList, Log, TEXT("rebuild samplers"));
	m_SSamplers->ClearChildren();

	const UPopcornFXAttributeList		*attrList = AttrList();
	if (attrList == null)
		return;

	const uint32			samplerCount = attrList->SamplerCount();

	for (hh_u32 sampleri = 0; sampleri < samplerCount; ++sampleri)
	{
		const FPopcornFXSamplerDesc		*desc = attrList->GetSamplerDesc(sampleri);
		if (!HH_VERIFY(desc != null) || desc->m_SamplerType == EPopcornFXAttributeSamplerType::None)
			continue;
		TSharedPtr<SAttributeSamplerDesc>		sdesc;
		m_SSamplers->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(sdesc, SAttributeSamplerDesc)
				.Parent(SharedThis(this))
				.SamplerI(sampleri)
			]
		;
		sdesc = null;
	}
}

EVisibility		FPopcornFXDetailsAttributeList::AttribVisibilityAndRefresh()
{
	const UPopcornFXAttributeList		*attrList = AttrList(); // will enqueue a refresh IFN
	(void)attrList;
	return EVisibility::Visible;
}

void	FPopcornFXDetailsAttributeList::CustomizeDetails(IDetailLayoutBuilder& detailLayout)
{
	//m_DetailBuilder = &detailLayout;
	m_PropertyUtilities = detailLayout.GetPropertyUtilities();
	detailLayout.GetObjectsBeingCustomized(m_BeingCustomized);

	// Not used ?
	// const UPopcornFXAttributeList	*attrList = UnsafeAttrList(); // AttrList() will ask for rebuild ifn, we dont want that here

	IDetailCategoryBuilder			&attrListCategory = detailLayout.EditCategory("PopcornFX Attributes");

	detailLayout.HideProperty("m_FileVersionId");
	detailLayout.HideProperty("m_Attributes");
	detailLayout.HideProperty("m_Samplers");
	detailLayout.HideProperty("m_AttributesRawData");

	m_FileVersionIdPty = detailLayout.GetProperty("m_FileVersionId");
	if (!HH_VERIFY(IsValidHandle(m_FileVersionIdPty)))
		return;

	m_SamplersPty = detailLayout.GetProperty("m_Samplers");
	if (!HH_VERIFY(IsValidHandle(m_SamplersPty)))
		return;

	// Hacky callback to trigger refresh.
	// Added to fix refresh when effect without attributes was re-imported with attribute.
	TAttribute<EVisibility>		attribVisibilityAndRefresh =
		TAttribute<EVisibility>::Create(
			TAttribute<EVisibility>::FGetter::CreateSP(this, &FPopcornFXDetailsAttributeList::AttribVisibilityAndRefresh));

	attrListCategory.AddCustomRow(LOCTEXT("Attributes", "Attributes"), false)
		.Visibility(attribVisibilityAndRefresh)
		.WholeRowContent()
		//.MinDesiredWidth(125.0f * 3.0f)
		//.MaxDesiredWidth(125.0f * 4.0f)
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[SAssignNew(m_SAttributes, SVerticalBox)]

			+ SVerticalBox::Slot()
				.FillHeight(1)
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[SAssignNew(m_SSamplers, SVerticalBox)]
		]
	;

	Rebuild();

	{
		FSimpleDelegate		refreshAttrs = FSimpleDelegate::CreateSP(this, &FPopcornFXDetailsAttributeList::RebuildAndRefresh);
		m_SamplersPty->AsArray()->SetOnNumElementsChanged(refreshAttrs);
	}
	{
		// It does not seem to work (this is why AttribVisibilityAndRefresh exists)
		FSimpleDelegate		refreshAttrs = FSimpleDelegate::CreateSP(this, &FPopcornFXDetailsAttributeList::Rebuild);
		m_FileVersionIdPty->SetOnPropertyValueChanged(refreshAttrs);
	}
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
