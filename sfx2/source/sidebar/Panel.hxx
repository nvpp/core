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
#ifndef INCLUDED_SFX2_SOURCE_SIDEBAR_PANEL_HXX
#define INCLUDED_SFX2_SOURCE_SIDEBAR_PANEL_HXX

#include "Context.hxx"
#include <vcl/window.hxx>

#include <com/sun/star/ui/XUIElement.hpp>
#include <com/sun/star/ui/XSidebarPanel.hpp>

#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace sfx2 { namespace sidebar {

class PanelDescriptor;
class TitleBar;
class PanelTitleBar;

class Panel
    : public vcl::Window
{
public:
    Panel (
        const PanelDescriptor& rPanelDescriptor,
        vcl::Window* pParentWindow,
        const bool bIsInitiallyExpanded,
        const ::boost::function<void()>& rDeckLayoutTrigger,
        const ::boost::function<Context()>& rContextAccess);
    virtual ~Panel();
    virtual void dispose() SAL_OVERRIDE;

    PanelTitleBar* GetTitleBar() const;
    bool IsTitleBarOptional() const { return mbIsTitleBarOptional;}
    void SetUIElement (const css::uno::Reference<css::ui::XUIElement>& rxElement);
    css::uno::Reference<css::ui::XSidebarPanel> GetPanelComponent() const { return mxPanelComponent;}
    css::uno::Reference<css::awt::XWindow> GetElementWindow();
    void SetExpanded (const bool bIsExpanded);
    bool IsExpanded() const { return mbIsExpanded;}
    bool HasIdPredicate (const ::rtl::OUString& rsId) const;
    const ::rtl::OUString& GetId() const { return msPanelId;}

    virtual void Paint (vcl::RenderContext& rRenderContext, const Rectangle& rUpdateArea) SAL_OVERRIDE;
    virtual void Resize() SAL_OVERRIDE;
    virtual void DataChanged (const DataChangedEvent& rEvent) SAL_OVERRIDE;
    virtual void Activate() SAL_OVERRIDE;

private:
    const ::rtl::OUString msPanelId;
    VclPtr<PanelTitleBar> mpTitleBar;
    const bool mbIsTitleBarOptional;
    css::uno::Reference<css::ui::XUIElement> mxElement;
    css::uno::Reference<css::ui::XSidebarPanel> mxPanelComponent;
    bool mbIsExpanded;
    const ::boost::function<void()> maDeckLayoutTrigger;
    const ::boost::function<Context()> maContextAccess;
};
typedef ::std::vector< VclPtr< Panel > > SharedPanelContainer;

} } // end of namespace sfx2::sidebar

#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
