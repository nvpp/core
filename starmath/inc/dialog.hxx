/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */
#ifndef INCLUDED_STARMATH_INC_DIALOG_HXX
#define INCLUDED_STARMATH_INC_DIALOG_HXX

#include <vcl/image.hxx>
#include <vcl/dialog.hxx>
#include <vcl/fixed.hxx>
#include <vcl/button.hxx>
#include <vcl/layout.hxx>
#include <sfx2/tabdlg.hxx>
#include <vcl/combobox.hxx>
#include <svx/charmap.hxx>
#include <sfx2/basedlgs.hxx>
#include <vcl/field.hxx>
#include <vcl/menubtn.hxx>
#include <vcl/scrbar.hxx>
#include <vcl/ctrl.hxx>
#include <vcl/menu.hxx>
#include <vcl/outdev.hxx>
#include <svtools/ctrlbox.hxx>
#include <svtools/ctrltool.hxx>
#include "utility.hxx"
#include "format.hxx"
#include "symbol.hxx"
#include <memory>

class SubsetMap;
#define CATEGORY_NONE   0xFFFF

/**************************************************************************/

void SetFontStyle(const OUString &rStyleName, vcl::Font &rFont);

/**************************************************************************/

class SmPrintOptionsTabPage : public SfxTabPage
{
    VclPtr<CheckBox>       m_pTitle;
    VclPtr<CheckBox>       m_pText;
    VclPtr<CheckBox>       m_pFrame;
    VclPtr<RadioButton>    m_pSizeNormal;
    VclPtr<RadioButton>    m_pSizeScaled;
    VclPtr<RadioButton>    m_pSizeZoomed;
    VclPtr<MetricField>    m_pZoom;
    VclPtr<CheckBox>       m_pNoRightSpaces;
    VclPtr<CheckBox>       m_pSaveOnlyUsedSymbols;

    DECL_LINK(SizeButtonClickHdl, Button *);

    virtual bool    FillItemSet(SfxItemSet* rSet) SAL_OVERRIDE;
    virtual void    Reset(const SfxItemSet* rSet) SAL_OVERRIDE;

public:
    static VclPtr<SfxTabPage> Create(vcl::Window *pWindow, const SfxItemSet &rSet);

    SmPrintOptionsTabPage(vcl::Window *pParent, const SfxItemSet &rOptions);
    virtual ~SmPrintOptionsTabPage();
    virtual void dispose() SAL_OVERRIDE;
};

/**************************************************************************/

class SmShowFont : public vcl::Window
{
    virtual void Paint(vcl::RenderContext& rRenderContext, const Rectangle&) SAL_OVERRIDE;

public:
    SmShowFont(vcl::Window *pParent, WinBits nStyle)
        : Window(pParent, nStyle)
    {
    }
    virtual Size GetOptimalSize() const SAL_OVERRIDE;
    void SetFont(const vcl::Font& rFont);
};

class SmFontDialog : public ModalDialog
{
    VclPtr<ComboBox>       m_pFontBox;
    VclPtr<VclContainer>   m_pAttrFrame;
    VclPtr<CheckBox>       m_pBoldCheckBox;
    VclPtr<CheckBox>       m_pItalicCheckBox;
    VclPtr<SmShowFont>     m_pShowFont;

    vcl::Font       Face;

    DECL_LINK(FontSelectHdl, ComboBox *);
    DECL_LINK(FontModifyHdl, ComboBox *);
    DECL_LINK(AttrChangeHdl, CheckBox *);

    void            InitColor_Impl();

    virtual void    DataChanged( const DataChangedEvent& rDCEvt ) SAL_OVERRIDE;

public:
    SmFontDialog(vcl::Window * pParent, OutputDevice *pFntListDevice, bool bHideCheckboxes);
    virtual ~SmFontDialog();
    virtual void dispose() SAL_OVERRIDE;

    const vcl::Font& GetFont() const { return Face; }
    void        SetFont(const vcl::Font &rFont);
};

/**************************************************************************/

class SmFontSizeDialog : public ModalDialog
{
    VclPtr<MetricField> m_pBaseSize;
    VclPtr<MetricField> m_pTextSize;
    VclPtr<MetricField> m_pIndexSize;
    VclPtr<MetricField> m_pFunctionSize;
    VclPtr<MetricField> m_pOperatorSize;
    VclPtr<MetricField> m_pBorderSize;
    VclPtr<PushButton> m_pDefaultButton;

    DECL_LINK(DefaultButtonClickHdl, Button *);

public:
    SmFontSizeDialog(vcl::Window *pParent);
    virtual ~SmFontSizeDialog();
    virtual void dispose() SAL_OVERRIDE;

    void ReadFrom(const SmFormat &rFormat);
    void WriteTo (SmFormat &rFormat) const;
};

/**************************************************************************/

class SmFontTypeDialog : public ModalDialog
{
    VclPtr<SmFontPickListBox> m_pVariableFont;
    VclPtr<SmFontPickListBox> m_pFunctionFont;
    VclPtr<SmFontPickListBox> m_pNumberFont;
    VclPtr<SmFontPickListBox> m_pTextFont;
    VclPtr<SmFontPickListBox> m_pSerifFont;
    VclPtr<SmFontPickListBox> m_pSansFont;
    VclPtr<SmFontPickListBox> m_pFixedFont;
    VclPtr<MenuButton> m_pMenuButton;
    VclPtr<PushButton> m_pDefaultButton;

    VclPtr<OutputDevice> pFontListDev;

    DECL_LINK(MenuSelectHdl, Menu *);
    DECL_LINK(DefaultButtonClickHdl, Button *);
    DECL_LINK(HelpButtonClickHdl, Button *);

public:
    SmFontTypeDialog(vcl::Window *pParent, OutputDevice *pFntListDevice);
    virtual ~SmFontTypeDialog();
    virtual void dispose() SAL_OVERRIDE;

    void ReadFrom(const SmFormat &rFormat);
    void WriteTo (SmFormat &rFormat) const;
};

/**************************************************************************/

#define NOCATEGORIES    10

class SmCategoryDesc
{
    OUString Name;
    OUString  *Strings[4];
    Image *Graphics[4];    /* regular bitmaps */
    sal_uInt16 Minimum[4];
    sal_uInt16 Maximum[4];
    sal_uInt16 Value[4];

public:
    SmCategoryDesc(VclBuilderContainer& rBuilder, sal_uInt16 nCategoryIdx);
    ~SmCategoryDesc();

    const OUString& GetName() const                 { return Name; }
    const OUString* GetString(sal_uInt16 Index) const   { return Strings[Index];  }
    sal_uInt16          GetMinimum(sal_uInt16 Index)        { return Minimum[Index]; }
    sal_uInt16          GetMaximum(sal_uInt16 Index)        { return Maximum[Index]; }
    sal_uInt16          GetValue(sal_uInt16 Index) const    { return Value[Index]; }
    void            SetValue(sal_uInt16 Index, sal_uInt16 nVal) { Value[Index] = nVal;}

    const Image *  GetGraphic(sal_uInt16 Index) const
    {
        return Graphics[Index];
    }
};


class SmDistanceDialog : public ModalDialog
{
    VclPtr<VclFrame>       m_pFrame;
    VclPtr<FixedText>      m_pFixedText1;
    VclPtr<MetricField>    m_pMetricField1;
    VclPtr<FixedText>      m_pFixedText2;
    VclPtr<MetricField>    m_pMetricField2;
    VclPtr<FixedText>      m_pFixedText3;
    VclPtr<MetricField>    m_pMetricField3;
    VclPtr<CheckBox>       m_pCheckBox1;
    VclPtr<FixedText>      m_pFixedText4;
    VclPtr<MetricField>    m_pMetricField4;
    VclPtr<MenuButton>     m_pMenuButton;
    VclPtr<PushButton>     m_pDefaultButton;
    VclPtr<FixedImage>     m_pBitmap;

    SmCategoryDesc *Categories[NOCATEGORIES];
    sal_uInt16          nActiveCategory;
    bool            bScaleAllBrackets;

    DECL_LINK(GetFocusHdl, Control *);
    DECL_LINK(MenuSelectHdl, Menu *);
    DECL_LINK(DefaultButtonClickHdl, Button *);
    DECL_LINK(CheckBoxClickHdl, CheckBox *);

    using   Window::SetHelpId;
    static void SetHelpId(MetricField &rField, const OString& sHelpId);
    void    SetCategory(sal_uInt16 Category);

public:
    SmDistanceDialog(vcl::Window *pParent);
    virtual ~SmDistanceDialog();
    virtual void dispose() SAL_OVERRIDE;

    void ReadFrom(const SmFormat &rFormat);
    void WriteTo (SmFormat &rFormat) /*const*/;

    // Window
    virtual void    DataChanged( const DataChangedEvent &rEvt ) SAL_OVERRIDE;
};


/**************************************************************************/


class SmAlignDialog : public ModalDialog
{
    VclPtr<RadioButton> m_pLeft;
    VclPtr<RadioButton> m_pCenter;
    VclPtr<RadioButton> m_pRight;
    VclPtr<PushButton>  m_pDefaultButton;

    DECL_LINK(DefaultButtonClickHdl, Button *);

public:
    SmAlignDialog(vcl::Window *pParent);
    virtual ~SmAlignDialog();
    virtual void dispose() SAL_OVERRIDE;

    void ReadFrom(const SmFormat &rFormat);
    void WriteTo (SmFormat &rFormat) const;
};

/**************************************************************************/

class SmShowSymbolSetWindow : public Control
{
    VclPtr<ScrollBar>  m_pVScrollBar;
    SymbolPtrVec_t aSymbolSet;
    Link<>      aSelectHdlLink;
    Link<>      aDblClickHdlLink;
    long        nLen;
    long        nRows, nColumns;
    long        nXOffset, nYOffset;
    sal_uInt16  nSelectSymbol;

    void SetScrollBarRange();
    Point OffsetPoint(const Point &rPoint) const;

    virtual void    Paint(vcl::RenderContext& rRenderContext, const Rectangle&) SAL_OVERRIDE;
    virtual void    MouseButtonDown(const MouseEvent& rMEvt) SAL_OVERRIDE;
    virtual void    KeyInput(const KeyEvent& rKEvt) SAL_OVERRIDE;
    virtual void    Resize() SAL_OVERRIDE;
    virtual Size    GetOptimalSize() const SAL_OVERRIDE;

    DECL_LINK( ScrollHdl, ScrollBar* );
public:
    SmShowSymbolSetWindow(vcl::Window *pParent, WinBits nStyle);
    virtual ~SmShowSymbolSetWindow();
    virtual void dispose() SAL_OVERRIDE;
    void setScrollbar(ScrollBar *pVScrollBar);
    void calccols();
    void    SelectSymbol(sal_uInt16 nSymbol);
    sal_uInt16  GetSelectSymbol() const { return nSelectSymbol; }
    void SetSymbolSet(const SymbolPtrVec_t& rSymbolSet);
    void SetSelectHdl(const Link<>& rLink) { aSelectHdlLink = rLink; }
    void SetDblClickHdl(const Link<>& rLink) { aDblClickHdlLink = rLink; }
};

class SmShowSymbolSet : public VclHBox
{
    VclPtr<SmShowSymbolSetWindow> aSymbolWindow;
    VclPtr<ScrollBar>             aVScrollBar;

public:
    SmShowSymbolSet(vcl::Window *pParent);
    virtual ~SmShowSymbolSet();
    virtual void dispose() SAL_OVERRIDE;

    void    SetSymbolSet(const SymbolPtrVec_t& rSymbolSet) { aSymbolWindow->SetSymbolSet(rSymbolSet); }

    void    SelectSymbol(sal_uInt16 nSymbol) { aSymbolWindow->SelectSymbol(nSymbol); }
    sal_uInt16  GetSelectSymbol() const { return aSymbolWindow->GetSelectSymbol(); }

    void SetSelectHdl(const Link<>& rLink) { aSymbolWindow->SetSelectHdl(rLink); }
    void SetDblClickHdl(const Link<>& rLink) { aSymbolWindow->SetDblClickHdl(rLink); }
};



class SmShowSymbol : public Control
{
    Link<> aDblClickHdlLink;

    virtual void    Paint(vcl::RenderContext& rRenderContext, const Rectangle&) SAL_OVERRIDE;
    virtual void    MouseButtonDown(const MouseEvent& rMEvt) SAL_OVERRIDE;
    virtual void    Resize() SAL_OVERRIDE;

    void setFontSize(vcl::Font &rFont) const;

public:
    SmShowSymbol(vcl::Window *pParent, WinBits nStyle)
        : Control(pParent, nStyle)
    {
    }

    void    SetSymbol(const SmSym *pSymbol);
    void    SetDblClickHdl(const Link<> &rLink) { aDblClickHdlLink = rLink; }
};



class SmSymDefineDialog;

class SmSymbolDialog : public ModalDialog
{
    VclPtr<ListBox>         m_pSymbolSets;
    VclPtr<SmShowSymbolSet> m_pSymbolSetDisplay;
    VclPtr<FixedText>       m_pSymbolName;
    VclPtr<SmShowSymbol>    m_pSymbolDisplay;
    VclPtr<PushButton>      m_pGetBtn;
    VclPtr<PushButton>      m_pEditBtn;

    SmViewShell        &rViewSh;
    SmSymbolManager    &rSymbolMgr;

    OUString       aSymbolSetName;
    SymbolPtrVec_t      aSymbolSet;

    VclPtr<OutputDevice> pFontListDev;

    DECL_LINK(SymbolSetChangeHdl, void*);
    DECL_LINK(SymbolChangeHdl, void*);
    DECL_LINK(SymbolDblClickHdl, void*);
    DECL_LINK(EditClickHdl, void*);
    DECL_LINK(GetClickHdl, void*);

    void            FillSymbolSets(bool bDeleteText = true);
    void            SetSymbolSetManager(SmSymbolManager &rMgr);
    const SmSym    *GetSymbol() const;
    void            InitColor_Impl();

    virtual void    DataChanged( const DataChangedEvent& rDCEvt ) SAL_OVERRIDE;

public:
    SmSymbolDialog(vcl::Window * pParent, OutputDevice *pFntListDevice,
            SmSymbolManager &rSymbolMgr, SmViewShell &rViewShell);
    virtual ~SmSymbolDialog();
    virtual void dispose() SAL_OVERRIDE;

    bool    SelectSymbolSet(const OUString &rSymbolSetName);
    void    SelectSymbol(sal_uInt16 nSymbolPos);
    sal_uInt16  GetSelectedSymbol() const   { return m_pSymbolSetDisplay->GetSelectSymbol(); }
};



class SmShowChar : public Control
{
    virtual void    Paint(vcl::RenderContext& rRenderContext, const Rectangle&) SAL_OVERRIDE;
    virtual void    Resize() SAL_OVERRIDE;

public:
    SmShowChar(vcl::Window *pParent, WinBits nStyle)
    : Control(pParent, nStyle)
    {
    }

    void    SetSymbol( const SmSym *pSym );
    void    SetSymbol( sal_UCS4 cChar, const vcl::Font &rFont );
};



class SmSymDefineDialog : public ModalDialog
{
    VclPtr<ComboBox>        pOldSymbols;
    VclPtr<ComboBox>        pOldSymbolSets;
    VclPtr<SvxShowCharSet>  pCharsetDisplay;
    VclPtr<ComboBox>        pSymbols;
    VclPtr<ComboBox>        pSymbolSets;
    VclPtr<ListBox>         pFonts;
    VclPtr<ListBox>         pFontsSubsetLB;
    VclPtr<FontStyleBox>    pStyles;
    VclPtr<FixedText>       pOldSymbolName;
    VclPtr<SmShowChar>      pOldSymbolDisplay;
    VclPtr<FixedText>       pOldSymbolSetName;
    VclPtr<FixedText>       pSymbolName;
    VclPtr<SmShowChar>      pSymbolDisplay;
    VclPtr<FixedText>       pSymbolSetName;
    VclPtr<PushButton>      pAddBtn;
    VclPtr<PushButton>      pChangeBtn;
    VclPtr<PushButton>      pDeleteBtn;

    SmSymbolManager     aSymbolMgrCopy,
                       &rSymbolMgr;
    std::unique_ptr<SmSym> pOrigSymbol;

    std::unique_ptr<SubsetMap> pSubsetMap;
    FontList           *pFontList;

    DECL_LINK(OldSymbolChangeHdl, ComboBox *);
    DECL_LINK(OldSymbolSetChangeHdl, ComboBox *);
    DECL_LINK(ModifyHdl, ComboBox *);
    DECL_LINK(FontChangeHdl, ListBox *);
    DECL_LINK(SubsetChangeHdl, ListBox*);
    DECL_LINK(StyleChangeHdl, ComboBox *);
    DECL_LINK(CharHighlightHdl, void *);
    DECL_LINK(AddClickHdl, Button *);
    DECL_LINK(ChangeClickHdl, Button *);
    DECL_LINK(DeleteClickHdl, Button *);

    void    FillSymbols(ComboBox &rComboBox, bool bDeleteText = true);
    void    FillSymbolSets(ComboBox &rComboBox, bool bDeleteText = true);
    void    FillFonts(bool bDeleteText = true);
    void    FillStyles(bool bDeleteText = true);

    void    SetSymbolSetManager(const SmSymbolManager &rMgr);
    void    SetFont(const OUString &rFontName, const OUString &rStyleName);
    void    SetOrigSymbol(const SmSym *pSymbol, const OUString &rSymbolSetName);
    void    UpdateButtons();

    bool    SelectSymbolSet(ComboBox &rComboBox, const OUString &rSymbolSetName,
                            bool bDeleteText);
    bool    SelectSymbol(ComboBox &rComboBox, const OUString &rSymbolName,
                            bool bDeleteText);
    bool    SelectFont(const OUString &rFontName, bool bApplyFont);
    bool    SelectStyle(const OUString &rStyleName, bool bApplyFont);

    SmSym       * GetSymbol(const ComboBox &rComboBox);
    const SmSym * GetSymbol(const ComboBox &rComboBox) const
    {
        return const_cast<SmSymDefineDialog *>(this)->GetSymbol(rComboBox);
    }

    void            InitColor_Impl();

    virtual void    DataChanged( const DataChangedEvent& rDCEvt ) SAL_OVERRIDE;

public:
    SmSymDefineDialog(vcl::Window *pParent, OutputDevice *pFntListDevice, SmSymbolManager &rMgr);
    virtual ~SmSymDefineDialog();
    virtual void dispose() SAL_OVERRIDE;

    using OutputDevice::SetFont;

    // Dialog
    virtual short   Execute() SAL_OVERRIDE;

    bool SelectOldSymbolSet(const OUString &rSymbolSetName)
    {
        return SelectSymbolSet(*pOldSymbolSets, rSymbolSetName, false);
    }

    bool SelectOldSymbol(const OUString &rSymbolName)
    {
        return SelectSymbol(*pOldSymbols, rSymbolName, false);
    }

    bool SelectSymbolSet(const OUString &rSymbolSetName)
    {
        return SelectSymbolSet(*pSymbolSets, rSymbolSetName, false);
    }

    bool SelectSymbol(const OUString &rSymbolName)
    {
        return SelectSymbol(*pSymbols, rSymbolName, false);
    }

    bool        SelectFont(const OUString &rFontName)   { return SelectFont(rFontName, true); }
    bool        SelectStyle(const OUString &rStyleName) { return SelectStyle(rStyleName, true); };
    void        SelectChar(sal_Unicode cChar);
};



#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
