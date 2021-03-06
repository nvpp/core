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

#ifndef INCLUDED_AVMEDIA_MEDIAITEM_HXX
#define INCLUDED_AVMEDIA_MEDIAITEM_HXX

#include <tools/rtti.hxx>
#include <svl/poolitem.hxx>
#include <com/sun/star/media/ZoomLevel.hpp>
#include <com/sun/star/frame/XModel.hpp>
#include <com/sun/star/embed/XStorage.hpp>
#include <avmedia/avmediadllapi.h>
#include <memory>
#include <o3tl/typed_flags_set.hxx>

enum class AVMediaSetMask
{
    NONE        = 0x000,
    STATE       = 0x001,
    DURATION    = 0x002,
    TIME        = 0x004,
    LOOP        = 0x008,
    MUTE        = 0x010,
    VOLUMEDB    = 0x020,
    ZOOM        = 0x040,
    URL         = 0x080,
    MIME_TYPE   = 0x100,
    ALL         = 0x1ff,
};
namespace o3tl
{
    template<> struct typed_flags<AVMediaSetMask> : is_typed_flags<AVMediaSetMask, 0x1ff> {};
}

class SvStream;

namespace avmedia
{


// - MediaState -


enum MediaState
{
    MEDIASTATE_STOP = 0,
    MEDIASTATE_PLAY = 1,
    MEDIASTATE_PAUSE = 2
};


// - MediaItem -


class AVMEDIA_DLLPUBLIC MediaItem : public SfxPoolItem
{
public:
                            TYPEINFO_OVERRIDE();

    explicit                MediaItem( sal_uInt16 i_nWhich = 0,
                                       AVMediaSetMask nMaskSet = AVMediaSetMask::NONE );
                            MediaItem( const MediaItem& rMediaItem );
    virtual                 ~MediaItem();

    virtual bool            operator==( const SfxPoolItem& ) const SAL_OVERRIDE;
    virtual SfxPoolItem*    Clone( SfxItemPool* pPool = 0 ) const SAL_OVERRIDE;
    virtual bool GetPresentation( SfxItemPresentation ePres,
                                                 SfxMapUnit eCoreUnit,
                                                 SfxMapUnit ePresUnit,
                                                 OUString&  rText,
                                                 const IntlWrapper *pIntl ) const SAL_OVERRIDE;
    virtual bool            QueryValue( com::sun::star::uno::Any& rVal, sal_uInt8 nMemberId = 0 ) const SAL_OVERRIDE;
    virtual bool            PutValue( const com::sun::star::uno::Any& rVal, sal_uInt8 nMemberId = 0 ) SAL_OVERRIDE;

    void                    merge( const MediaItem& rMediaItem );

    AVMediaSetMask          getMaskSet() const;

    void                    setState( MediaState eState );
    MediaState              getState() const;

    void                    setDuration( double fDuration );
    double                  getDuration() const;

    void                    setTime( double fTime );
    double                  getTime() const;

    void                    setLoop( bool bLoop );
    bool                    isLoop() const;

    void                    setMute( bool bMute );
    bool                    isMute() const;

    void                    setVolumeDB( sal_Int16 nDB );
    sal_Int16               getVolumeDB() const;

    void                    setZoom( ::com::sun::star::media::ZoomLevel eZoom );
    ::com::sun::star::media::ZoomLevel  getZoom() const;

    void                    setURL( const OUString& rURL,
                                    const OUString& rTempURL,
                                    const OUString& rReferer);
    const OUString&  getURL() const;

    void                    setMimeType( const OUString& rMimeType );
    OUString                getMimeType() const;
    const OUString&  getTempURL() const;

    const OUString&  getReferer() const;

private:

    struct Impl;
    std::unique_ptr<Impl> m_pImpl;
};

typedef ::avmedia::MediaItem avmedia_MediaItem;

bool AVMEDIA_DLLPUBLIC EmbedMedia(
        ::com::sun::star::uno::Reference< ::com::sun::star::frame::XModel>
            const& xModel,
        OUString const& rSourceURL,
        OUString & o_rEmbeddedURL);

OUString GetFilename(OUString const& rSourceURL);

::com::sun::star::uno::Reference< ::com::sun::star::io::XStream> CreateStream(
    ::com::sun::star::uno::Reference< ::com::sun::star::embed::XStorage> const& xStorage, OUString const& rFilename);
}

#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
