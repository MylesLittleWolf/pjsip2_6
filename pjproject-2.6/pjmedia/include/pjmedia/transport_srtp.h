/* $Id: transport_srtp.h 5412 2016-08-08 09:09:29Z ming $ */
/* 
 * Copyright (C) 2008-2011 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
 */
#ifndef __PJMEDIA_TRANSPORT_SRTP_H__
#define __PJMEDIA_TRANSPORT_SRTP_H__

/**
 * @file transport_srtp.h
 * @brief Secure RTP (SRTP) transport.
 */

#include <pjmedia/transport.h>


/**
 * @defgroup PJMEDIA_TRANSPORT_SRTP Secure RTP (SRTP) Media Transport
 * @ingroup PJMEDIA_TRANSPORT
 * @brief Media transport adapter to add SRTP feature to existing transports
 * @{
 *
 * This module implements SRTP as described by RFC 3711, using RFC 4568 as
 * key exchange method. It implements \ref PJMEDIA_TRANSPORT to integrate
 * with the rest of PJMEDIA framework.
 *
 * As we know, media transport is separated from the stream object (which 
 * does the encoding/decoding of PCM frames, (de)packetization of RTP/RTCP 
 * packets, and de-jitter buffering). The connection between stream and media
 * transport is established when the stream is created (we need to specify 
 * media transport during stream creation), and the interconnection can be 
 * depicted from the diagram below:
 *
   \image html media-transport.PNG

 * I think the diagram above is self-explanatory.
 *
 * SRTP functionality is implemented as some kind of "adapter", which is 
 * plugged between the stream and the actual media transport that does 
 * sending/receiving RTP/RTCP packets. When SRTP is used, the interconnection
 * between stream and transport is like the diagram below:
 *
    \image html media-srtp-transport.PNG

 * So to stream, the SRTP transport behaves as if it is a media transport 
 * (because it is a media transport), and to the media transport it behaves
 * as if it is a stream. The SRTP object then forwards RTP packets back and
 * forth between stream and the actual transport, encrypting/decrypting 
 * the RTP/RTCP packets as necessary.
 * 
 * The neat thing about this design is the SRTP "adapter" then can be used 
 * to encrypt any kind of media transports. We currently have UDP and ICE 
 * media transports that can benefit SRTP, and we could add SRTP to any 
 * media transports that will be added in the future. 
 */

PJ_BEGIN_DECL


/**
 * Crypto option.
 */
typedef enum pjmedia_srtp_crypto_option
{
    /** When this flag is specified, encryption will be disabled. */
    PJMEDIA_SRTP_NO_ENCRYPTION  = 1,

    /** When this flag is specified, authentication will be disabled. */
    PJMEDIA_SRTP_NO_AUTHENTICATION  = 2

} pjmedia_srtp_crypto_option;


/**
 * This structure describes an individual crypto setting.
 */
typedef struct pjmedia_srtp_crypto
{
    /** Optional key. If empty, a random key will be autogenerated. */
    pj_str_t	key;

    /** Crypto name.   */
    pj_str_t	name;

    /** Flags, bitmask from #pjmedia_srtp_crypto_option */
    unsigned	flags;

} pjmedia_srtp_crypto;


/**
 * This enumeration specifies the behavior of the SRTP transport regarding
 * media security offer and answer.
 */
typedef enum pjmedia_srtp_use
{
    /**
     * When this flag is specified, SRTP will be disabled, and the transport
     * will reject RTP/SAVP offer.
     */
    PJMEDIA_SRTP_DISABLED,

    /**
     * When this flag is specified, SRTP will be advertised as optional and
     * incoming SRTP offer will be accepted.
     */
    PJMEDIA_SRTP_OPTIONAL,

    /**
     * When this flag is specified, the transport will require that RTP/SAVP
     * media shall be used.
     */
    PJMEDIA_SRTP_MANDATORY

} pjmedia_srtp_use;


/**
 * Settings to be given when creating SRTP transport. Application should call
 * #pjmedia_srtp_setting_default() to initialize this structure with its 
 * default values.
 */
typedef struct pjmedia_srtp_setting
{
    /**
     * Specify the usage policy. Default is PJMEDIA_SRTP_OPTIONAL.
     */
    pjmedia_srtp_use		use;

    /**
     * Specify whether the SRTP transport should close the member transport 
     * when it is destroyed. Default: PJ_TRUE.
     */
    pj_bool_t			close_member_tp;

    /**
     * Specify the number of crypto suite settings.
     */
    unsigned			crypto_count;

    /**
     * Specify individual crypto suite setting.
     */
    pjmedia_srtp_crypto		crypto[PJMEDIA_SRTP_MAX_CRYPTOS];

} pjmedia_srtp_setting;


/**
 * This structure specifies SRTP transport specific info. This will fit
 * into \a buffer field of pjmedia_transport_specific_info.
 */
typedef struct pjmedia_srtp_info
{
    /**
     * Specify whether the SRTP transport is active for SRTP session.
     */
    pj_bool_t			active;

    /**
     * Specify the policy used by the SRTP session for receive direction.
     */
    pjmedia_srtp_crypto		rx_policy;

    /**
     * Specify the policy used by the SRTP session for transmit direction.
     */
    pjmedia_srtp_crypto		tx_policy;

    /**
     * Specify the usage policy.
     */
    pjmedia_srtp_use		use;

    /**
     * Specify the peer's usage policy.
     */
    pjmedia_srtp_use		peer_use;

} pjmedia_srtp_info;


/**
 * Initialize SRTP library. This function should be called before
 * any SRTP functions, however calling #pjmedia_transport_srtp_create() 
 * will also invoke this function. This function will also register SRTP
 * library deinitialization to #pj_atexit(), so the deinitialization
 * of SRTP library will be performed automatically by PJLIB destructor.
 *
 * @param endpt	    The media endpoint instance.
 *
 * @return	    PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjmedia_srtp_init_lib(pjmedia_endpt *endpt);


/**
 * Initialize SRTP setting with its default values.
 *
 * @param opt	SRTP setting to be initialized.
 */
PJ_DECL(void) pjmedia_srtp_setting_default(pjmedia_srtp_setting *opt);


/**
 * Create an SRTP media transport.
 *
 * @param endpt	    The media endpoint instance.
 * @param tp	    The actual media transport to send and receive 
 *		    RTP/RTCP packets. This media transport will be
 *		    kept as member transport of this SRTP instance.
 * @param opt	    Optional settings. If NULL is given, default
 *		    settings will be used.
 * @param p_tp	    Pointer to receive the transport SRTP instance.
 *
 * @return	    PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjmedia_transport_srtp_create(
				       pjmedia_endpt *endpt,
				       pjmedia_transport *tp,
				       const pjmedia_srtp_setting *opt,
				       pjmedia_transport **p_tp);


/**
 * Manually start SRTP session with the given parameters. Application only
 * needs to call this function when the SRTP transport is used without SDP
 * offer/answer. When SDP offer/answer framework is used, the SRTP transport
 * will be started/stopped by #pjmedia_transport_media_start() and 
 * #pjmedia_transport_media_stop() respectively.
 *
 * Please note that even if an RTP stream is only one direction, application
 * will still need to provide both crypto suites, because it is needed by 
 * RTCP.

 * If application specifies the crypto keys, the keys for transmit and receive
 * direction MUST be different.
 *
 * @param srtp	    The SRTP transport.
 * @param tx	    Crypto suite setting for transmit direction.
 * @param rx	    Crypto suite setting for receive direction.
 *
 * @return	    PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjmedia_transport_srtp_start(
					    pjmedia_transport *srtp,
					    const pjmedia_srtp_crypto *tx,
					    const pjmedia_srtp_crypto *rx);

/**
 * Stop SRTP session.
 *
 * @param srtp	    The SRTP media transport.
 *
 * @return	    PJ_SUCCESS on success.
 *
 * @see #pjmedia_transport_srtp_start() 
 */
PJ_DECL(pj_status_t) pjmedia_transport_srtp_stop(pjmedia_transport *srtp);


/**
 * This is a utility function to decrypt SRTP packet using SRTP transport.
 * This function is not part of SRTP transport's API, but it can be used
 * to decrypt SRTP packets from non-network (for example, from a saved file)
 * without having to use the transport framework. See pcaputil.c in the
 * samples collection on how to use this function.
 *
 * @param tp		The SRTP transport.
 * @param is_rtp	Set to non-zero if the packet is SRTP, otherwise set
 *			to zero if the packet is SRTCP.
 * @param pkt		On input, it contains SRTP or SRTCP packet. On
 *			output, it contains the decrypted RTP/RTCP packet.
 * @param pkt_len	On input, specify the length of the buffer. On
 *			output, it will be filled with the actual length
 *			of decrypted packet.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjmedia_transport_srtp_decrypt_pkt(pjmedia_transport *tp,
							pj_bool_t is_rtp,
							void *pkt,
							int *pkt_len);


/**
 * Query member transport of SRTP.
 *
 * @param srtp		    The SRTP media transport.
 *
 * @return		    member media transport.
 */
PJ_DECL(pjmedia_transport*) pjmedia_transport_srtp_get_member(
						    pjmedia_transport *srtp);


PJ_END_DECL

/**
 * @}
 */

#endif /* __PJMEDIA_TRANSPORT_SRTP_H__ */
