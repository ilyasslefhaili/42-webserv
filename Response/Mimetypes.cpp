/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime_ex_type.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilefhail <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 15:37:46 by ilefhail          #+#    #+#             */
/*   Updated: 2023/03/09 15:37:47 by ilefhail         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Mimetypes.hpp"

MimeTypes::MimeTypes(){
    this->_ex_type.insert(std::make_pair("*3gpp", "audio/3gpp"));
    this->_ex_type.insert(std::make_pair("*jpm", "video/jpm"));
    this->_ex_type.insert(std::make_pair("*mp3", "audio/mp3"));
    this->_ex_type.insert(std::make_pair("*rtf", "text/rtf"));
    this->_ex_type.insert(std::make_pair("*wav", "audio/wave"));
    this->_ex_type.insert(std::make_pair("*xml", "text/xml"));
    this->_ex_type.insert(std::make_pair("3g2", "video/3gpp2"));
    this->_ex_type.insert(std::make_pair("3gp", "video/3gpp"));
    this->_ex_type.insert(std::make_pair("3gpp", "video/3gpp"));
    this->_ex_type.insert(std::make_pair("ac", "application/pkix-attr-cert"));
    this->_ex_type.insert(std::make_pair("adp", "audio/adpcm"));
    this->_ex_type.insert(std::make_pair("ai", "application/postscript"));
    this->_ex_type.insert(std::make_pair("apng", "image/apng"));
    this->_ex_type.insert(std::make_pair("appcache", "text/cache-manifest"));
    this->_ex_type.insert(std::make_pair("asc", "application/pgp-signature"));
    this->_ex_type.insert(std::make_pair("atom", "application/atom+xml"));
    this->_ex_type.insert(std::make_pair("atomcat", "application/atomcat+xml"));
    this->_ex_type.insert(std::make_pair("atomsvc", "application/atomsvc+xml"));
    this->_ex_type.insert(std::make_pair("au", "audio/basic"));
    this->_ex_type.insert(std::make_pair("aw", "application/applixware"));
    this->_ex_type.insert(std::make_pair("bdoc", "application/bdoc"));
    this->_ex_type.insert(std::make_pair("bin", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("bmp", "image/bmp"));
    this->_ex_type.insert(std::make_pair("bpk", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("buffer", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("ccxml", "application/ccxml+xml"));
    this->_ex_type.insert(std::make_pair("cdmia", "application/cdmi-capability"));
    this->_ex_type.insert(std::make_pair("cdmic", "application/cdmi-container"));
    this->_ex_type.insert(std::make_pair("cdmid", "application/cdmi-domain"));
    this->_ex_type.insert(std::make_pair("cdmio", "application/cdmi-object"));
    this->_ex_type.insert(std::make_pair("cdmiq", "application/cdmi-queue"));
    this->_ex_type.insert(std::make_pair("cer", "application/pkix-cert"));
    this->_ex_type.insert(std::make_pair("cgm", "image/cgm"));
    this->_ex_type.insert(std::make_pair("class", "application/java-vm"));
    this->_ex_type.insert(std::make_pair("coffee", "text/coffeescript"));
    this->_ex_type.insert(std::make_pair("conf", "text/plain"));
    this->_ex_type.insert(std::make_pair("cpt", "application/mac-compactpro"));
    this->_ex_type.insert(std::make_pair("crl", "application/pkix-crl"));
    this->_ex_type.insert(std::make_pair("css", "text/css"));
    this->_ex_type.insert(std::make_pair("csv", "text/csv"));
    this->_ex_type.insert(std::make_pair("cu", "application/cu-seeme"));
    this->_ex_type.insert(std::make_pair("davmount", "application/davmount+xml"));
    this->_ex_type.insert(std::make_pair("dbk", "application/docbook+xml"));
    this->_ex_type.insert(std::make_pair("deb", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("def", "text/plain"));
    this->_ex_type.insert(std::make_pair("deploy", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("disposition-notification", "message/disposition-notification"));
    this->_ex_type.insert(std::make_pair("dist", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("distz", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("dll", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("dmg", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("dms", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("doc", "application/msword"));
    this->_ex_type.insert(std::make_pair("dot", "application/msword"));
    this->_ex_type.insert(std::make_pair("drle", "image/dicom-rle"));
    this->_ex_type.insert(std::make_pair("dssc", "application/dssc+der"));
    this->_ex_type.insert(std::make_pair("dtd", "application/xml-dtd"));
    this->_ex_type.insert(std::make_pair("dump", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("ear", "application/java-archive"));
    this->_ex_type.insert(std::make_pair("ecma", "application/ecmascript"));
    this->_ex_type.insert(std::make_pair("elc", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("emf", "image/emf"));
    this->_ex_type.insert(std::make_pair("eml", "message/rfc822"));
    this->_ex_type.insert(std::make_pair("emma", "application/emma+xml"));
    this->_ex_type.insert(std::make_pair("eps", "application/postscript"));
    this->_ex_type.insert(std::make_pair("epub", "application/epub+zip"));
    this->_ex_type.insert(std::make_pair("es", "application/ecmascript"));
    this->_ex_type.insert(std::make_pair("exe", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("exi", "application/exi"));
    this->_ex_type.insert(std::make_pair("exr", "image/aces"));
    this->_ex_type.insert(std::make_pair("ez", "application/andrew-inset"));
    this->_ex_type.insert(std::make_pair("fits", "image/fits"));
    this->_ex_type.insert(std::make_pair("g3", "image/g3fax"));
    this->_ex_type.insert(std::make_pair("gbr", "application/rpki-ghostbusters"));
    this->_ex_type.insert(std::make_pair("geojson", "application/geo+json"));
    this->_ex_type.insert(std::make_pair("gif", "image/gif"));
    this->_ex_type.insert(std::make_pair("glb", "model/gltf-binary"));
    this->_ex_type.insert(std::make_pair("gltf", "model/gltf+json"));
    this->_ex_type.insert(std::make_pair("gml", "application/gml+xml"));
    this->_ex_type.insert(std::make_pair("gpx", "application/gpx+xml"));
    this->_ex_type.insert(std::make_pair("gram", "application/srgs"));
    this->_ex_type.insert(std::make_pair("grxml", "application/srgs+xml"));
    this->_ex_type.insert(std::make_pair("gxf", "application/gxf"));
    this->_ex_type.insert(std::make_pair("gz", "application/gzip"));
    this->_ex_type.insert(std::make_pair("h261", "video/h261"));
    this->_ex_type.insert(std::make_pair("h263", "video/h263"));
    this->_ex_type.insert(std::make_pair("h264", "video/h264"));
    this->_ex_type.insert(std::make_pair("heic", "image/heic"));
    this->_ex_type.insert(std::make_pair("heics", "image/heic-sequence"));
    this->_ex_type.insert(std::make_pair("heif", "image/heif"));
    this->_ex_type.insert(std::make_pair("heifs", "image/heif-sequence"));
    this->_ex_type.insert(std::make_pair("hjson", "application/hjson"));
    this->_ex_type.insert(std::make_pair("hlp", "application/winhlp"));
    this->_ex_type.insert(std::make_pair("hqx", "application/mac-binhex40"));
    this->_ex_type.insert(std::make_pair("htm", "text/html"));
    this->_ex_type.insert(std::make_pair("html", "text/html"));
    this->_ex_type.insert(std::make_pair("ics", "text/calendar"));
    this->_ex_type.insert(std::make_pair("ief", "image/ief"));
    this->_ex_type.insert(std::make_pair("ifb", "text/calendar"));
    this->_ex_type.insert(std::make_pair("iges", "model/iges"));
    this->_ex_type.insert(std::make_pair("igs", "model/iges"));
    this->_ex_type.insert(std::make_pair("img", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("in", "text/plain"));
    this->_ex_type.insert(std::make_pair("ini", "text/plain"));
    this->_ex_type.insert(std::make_pair("ink", "application/inkml+xml"));
    this->_ex_type.insert(std::make_pair("inkml", "application/inkml+xml"));
    this->_ex_type.insert(std::make_pair("ipfix", "application/ipfix"));
    this->_ex_type.insert(std::make_pair("iso", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("jade", "text/jade"));
    this->_ex_type.insert(std::make_pair("jar", "application/java-archive"));
    this->_ex_type.insert(std::make_pair("jls", "image/jls"));
    this->_ex_type.insert(std::make_pair("jpeg", "image/jpeg"));
    this->_ex_type.insert(std::make_pair("jpf", "image/jpx"));
    this->_ex_type.insert(std::make_pair("jpg", "image/jpeg"));
    this->_ex_type.insert(std::make_pair("jpg2", "image/jp2"));
    this->_ex_type.insert(std::make_pair("jpgm", "video/jpm"));
    this->_ex_type.insert(std::make_pair("jpgv", "video/jpeg"));
    this->_ex_type.insert(std::make_pair("jpm", "image/jpm"));
    this->_ex_type.insert(std::make_pair("jpx", "image/jpx"));
    this->_ex_type.insert(std::make_pair("js", "application/javascript"));
    this->_ex_type.insert(std::make_pair("json", "application/json"));
    this->_ex_type.insert(std::make_pair("json5", "application/json5"));
    this->_ex_type.insert(std::make_pair("jsonld", "application/ld+json"));
    this->_ex_type.insert(std::make_pair("jsonml", "application/jsonml+json"));
    this->_ex_type.insert(std::make_pair("jsx", "text/jsx"));
    this->_ex_type.insert(std::make_pair("kar", "audio/midi"));
    this->_ex_type.insert(std::make_pair("ktx", "image/ktx"));
    this->_ex_type.insert(std::make_pair("less", "text/less"));
    this->_ex_type.insert(std::make_pair("list", "text/plain"));
    this->_ex_type.insert(std::make_pair("litcoffee", "text/coffeescript"));
    this->_ex_type.insert(std::make_pair("log", "text/plain"));
    this->_ex_type.insert(std::make_pair("lostxml", "application/lost+xml"));
    this->_ex_type.insert(std::make_pair("lrf", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("m1v", "video/mpeg"));
    this->_ex_type.insert(std::make_pair("m21", "application/mp21"));
    this->_ex_type.insert(std::make_pair("m2a", "audio/mpeg"));
    this->_ex_type.insert(std::make_pair("m2v", "video/mpeg"));
    this->_ex_type.insert(std::make_pair("m3a", "audio/mpeg"));
    this->_ex_type.insert(std::make_pair("m4a", "audio/mp4"));
    this->_ex_type.insert(std::make_pair("m4p", "application/mp4"));
    this->_ex_type.insert(std::make_pair("ma", "application/mathematica"));
    this->_ex_type.insert(std::make_pair("mads", "application/mads+xml"));
    this->_ex_type.insert(std::make_pair("man", "text/troff"));
    this->_ex_type.insert(std::make_pair("manifest", "text/cache-manifest"));
    this->_ex_type.insert(std::make_pair("map", "application/json"));
    this->_ex_type.insert(std::make_pair("mar", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("markdown", "text/markdown"));
    this->_ex_type.insert(std::make_pair("mathml", "application/mathml+xml"));
    this->_ex_type.insert(std::make_pair("mb", "application/mathematica"));
    this->_ex_type.insert(std::make_pair("mbox", "application/mbox"));
    this->_ex_type.insert(std::make_pair("md", "text/markdown"));
    this->_ex_type.insert(std::make_pair("me", "text/troff"));
    this->_ex_type.insert(std::make_pair("mesh", "model/mesh"));
    this->_ex_type.insert(std::make_pair("meta4", "application/metalink4+xml"));
    this->_ex_type.insert(std::make_pair("metalink", "application/metalink+xml"));
    this->_ex_type.insert(std::make_pair("mets", "application/mets+xml"));
    this->_ex_type.insert(std::make_pair("mft", "application/rpki-manifest"));
    this->_ex_type.insert(std::make_pair("mid", "audio/midi"));
    this->_ex_type.insert(std::make_pair("midi", "audio/midi"));
    this->_ex_type.insert(std::make_pair("mime", "message/rfc822"));
    this->_ex_type.insert(std::make_pair("mj2", "video/mj2"));
    this->_ex_type.insert(std::make_pair("mjp2", "video/mj2"));
    this->_ex_type.insert(std::make_pair("mjs", "application/javascript"));
    this->_ex_type.insert(std::make_pair("mml", "text/mathml"));
    this->_ex_type.insert(std::make_pair("mods", "application/mods+xml"));
    this->_ex_type.insert(std::make_pair("mov", "video/quicktime"));
    this->_ex_type.insert(std::make_pair("mp2", "audio/mpeg"));
    this->_ex_type.insert(std::make_pair("mp21", "application/mp21"));
    this->_ex_type.insert(std::make_pair("mp2a", "audio/mpeg"));
    this->_ex_type.insert(std::make_pair("mp3", "audio/mpeg"));
    this->_ex_type.insert(std::make_pair("mp4", "video/mp4"));
    this->_ex_type.insert(std::make_pair("mp4a", "audio/mp4"));
    this->_ex_type.insert(std::make_pair("mp4s", "application/mp4"));
    this->_ex_type.insert(std::make_pair("mp4v", "video/mp4"));
    this->_ex_type.insert(std::make_pair("mpd", "application/dash+xml"));
    this->_ex_type.insert(std::make_pair("mpe", "video/mpeg"));
    this->_ex_type.insert(std::make_pair("mpeg", "video/mpeg"));
    this->_ex_type.insert(std::make_pair("mpg", "video/mpeg"));
    this->_ex_type.insert(std::make_pair("mpg4", "video/mp4"));
    this->_ex_type.insert(std::make_pair("mpga", "audio/mpeg"));
    this->_ex_type.insert(std::make_pair("mrc", "application/marc"));
    this->_ex_type.insert(std::make_pair("mrcx", "application/marcxml+xml"));
    this->_ex_type.insert(std::make_pair("ms", "text/troff"));
    this->_ex_type.insert(std::make_pair("mscml", "application/mediaservercontrol+xml"));
    this->_ex_type.insert(std::make_pair("msh", "model/mesh"));
    this->_ex_type.insert(std::make_pair("msi", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("msm", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("msp", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("mxf", "application/mxf"));
    this->_ex_type.insert(std::make_pair("mxml", "application/xv+xml"));
    this->_ex_type.insert(std::make_pair("n3", "text/n3"));
    this->_ex_type.insert(std::make_pair("nb", "application/mathematica"));
    this->_ex_type.insert(std::make_pair("oda", "application/oda"));
    this->_ex_type.insert(std::make_pair("oga", "audio/ogg"));
    this->_ex_type.insert(std::make_pair("ogg", "audio/ogg"));
    this->_ex_type.insert(std::make_pair("ogv", "video/ogg"));
    this->_ex_type.insert(std::make_pair("ogx", "application/ogg"));
    this->_ex_type.insert(std::make_pair("omdoc", "application/omdoc+xml"));
    this->_ex_type.insert(std::make_pair("onepkg", "application/onenote"));
    this->_ex_type.insert(std::make_pair("onetmp", "application/onenote"));
    this->_ex_type.insert(std::make_pair("onetoc", "application/onenote"));
    this->_ex_type.insert(std::make_pair("onetoc2", "application/onenote"));
    this->_ex_type.insert(std::make_pair("opf", "application/oebps-package+xml"));
    this->_ex_type.insert(std::make_pair("otf", "font/otf"));
    this->_ex_type.insert(std::make_pair("owl", "application/rdf+xml"));
    this->_ex_type.insert(std::make_pair("oxps", "application/oxps"));
    this->_ex_type.insert(std::make_pair("p10", "application/pkcs10"));
    this->_ex_type.insert(std::make_pair("p7c", "application/pkcs7-mime"));
    this->_ex_type.insert(std::make_pair("p7m", "application/pkcs7-mime"));
    this->_ex_type.insert(std::make_pair("p7s", "application/pkcs7-signature"));
    this->_ex_type.insert(std::make_pair("p8", "application/pkcs8"));
    this->_ex_type.insert(std::make_pair("pdf", "application/pdf"));
    this->_ex_type.insert(std::make_pair("pfr", "application/font-tdpfr"));
    this->_ex_type.insert(std::make_pair("pgp", "application/pgp-encrypted"));
    this->_ex_type.insert(std::make_pair("pkg", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("pki", "application/pkixcmp"));
    this->_ex_type.insert(std::make_pair("pkipath", "application/pkix-pkipath"));
    this->_ex_type.insert(std::make_pair("pls", "application/pls+xml"));
    this->_ex_type.insert(std::make_pair("png", "image/png"));
    this->_ex_type.insert(std::make_pair("prf", "application/pics-rules"));
    this->_ex_type.insert(std::make_pair("ps", "application/postscript"));
    this->_ex_type.insert(std::make_pair("pskcxml", "application/pskc+xml"));
    this->_ex_type.insert(std::make_pair("qt", "video/quicktime"));
    this->_ex_type.insert(std::make_pair("raml", "application/raml+yaml"));
    this->_ex_type.insert(std::make_pair("rdf", "application/rdf+xml"));
    this->_ex_type.insert(std::make_pair("rif", "application/reginfo+xml"));
    this->_ex_type.insert(std::make_pair("rl", "application/resource-lists+xml"));
    this->_ex_type.insert(std::make_pair("rld", "application/resource-lists-diff+xml"));
    this->_ex_type.insert(std::make_pair("rmi", "audio/midi"));
    this->_ex_type.insert(std::make_pair("rnc", "application/relax-ng-compact-syntax"));
    this->_ex_type.insert(std::make_pair("rng", "application/xml"));
    this->_ex_type.insert(std::make_pair("roa", "application/rpki-roa"));
    this->_ex_type.insert(std::make_pair("roff", "text/troff"));
    this->_ex_type.insert(std::make_pair("rq", "application/sparql-query"));
    this->_ex_type.insert(std::make_pair("rs", "application/rls-services+xml"));
    this->_ex_type.insert(std::make_pair("rsd", "application/rsd+xml"));
    this->_ex_type.insert(std::make_pair("rss", "application/rss+xml"));
    this->_ex_type.insert(std::make_pair("rtf", "application/rtf"));
    this->_ex_type.insert(std::make_pair("rtx", "text/richtext"));
    this->_ex_type.insert(std::make_pair("s3m", "audio/s3m"));
    this->_ex_type.insert(std::make_pair("sbml", "application/sbml+xml"));
    this->_ex_type.insert(std::make_pair("scq", "application/scvp-cv-request"));
    this->_ex_type.insert(std::make_pair("scs", "application/scvp-cv-response"));
    this->_ex_type.insert(std::make_pair("sdp", "application/sdp"));
    this->_ex_type.insert(std::make_pair("ser", "application/java-serialized-object"));
    this->_ex_type.insert(std::make_pair("setpay", "application/set-payment-initiation"));
    this->_ex_type.insert(std::make_pair("setreg", "application/set-registration-initiation"));
    this->_ex_type.insert(std::make_pair("sgi", "image/sgi"));
    this->_ex_type.insert(std::make_pair("sgm", "text/sgml"));
    this->_ex_type.insert(std::make_pair("sgml", "text/sgml"));
    this->_ex_type.insert(std::make_pair("shex", "text/shex"));
    this->_ex_type.insert(std::make_pair("shf", "application/shf+xml"));
    this->_ex_type.insert(std::make_pair("shtml", "text/html"));
    this->_ex_type.insert(std::make_pair("sig", "application/pgp-signature"));
    this->_ex_type.insert(std::make_pair("sil", "audio/silk"));
    this->_ex_type.insert(std::make_pair("silo", "model/mesh"));
    this->_ex_type.insert(std::make_pair("slim", "text/slim"));
    this->_ex_type.insert(std::make_pair("slm", "text/slim"));
    this->_ex_type.insert(std::make_pair("smi", "application/smil+xml"));
    this->_ex_type.insert(std::make_pair("smil", "application/smil+xml"));
    this->_ex_type.insert(std::make_pair("snd", "audio/basic"));
    this->_ex_type.insert(std::make_pair("so", "application/octet-stream"));
    this->_ex_type.insert(std::make_pair("spp", "application/scvp-vp-response"));
    this->_ex_type.insert(std::make_pair("spq", "application/scvp-vp-request"));
    this->_ex_type.insert(std::make_pair("spx", "audio/ogg"));
    this->_ex_type.insert(std::make_pair("sru", "application/sru+xml"));
    this->_ex_type.insert(std::make_pair("srx", "application/sparql-results+xml"));
    this->_ex_type.insert(std::make_pair("ssdl", "application/ssdl+xml"));
    this->_ex_type.insert(std::make_pair("ssml", "application/ssml+xml"));
    this->_ex_type.insert(std::make_pair("stk", "application/hyperstudio"));
    this->_ex_type.insert(std::make_pair("styl", "text/stylus"));
    this->_ex_type.insert(std::make_pair("stylus", "text/stylus"));
    this->_ex_type.insert(std::make_pair("svg", "image/svg+xml"));
    this->_ex_type.insert(std::make_pair("svgz", "image/svg+xml"));
    this->_ex_type.insert(std::make_pair("t", "text/troff"));
    this->_ex_type.insert(std::make_pair("t38", "image/t38"));
    this->_ex_type.insert(std::make_pair("tei", "application/tei+xml"));
    this->_ex_type.insert(std::make_pair("teicorpus", "application/tei+xml"));
    this->_ex_type.insert(std::make_pair("text", "text/plain"));
    this->_ex_type.insert(std::make_pair("tfi", "application/thraud+xml"));
    this->_ex_type.insert(std::make_pair("tfx", "image/tiff-fx"));
    this->_ex_type.insert(std::make_pair("tif", "image/tiff"));
    this->_ex_type.insert(std::make_pair("tiff", "image/tiff"));
    this->_ex_type.insert(std::make_pair("tr", "text/troff"));
    this->_ex_type.insert(std::make_pair("ts", "video/mp2t"));
    this->_ex_type.insert(std::make_pair("tsd", "application/timestamped-data"));
    this->_ex_type.insert(std::make_pair("tsv", "text/tab-separated-values"));
    this->_ex_type.insert(std::make_pair("ttc", "font/collection"));
    this->_ex_type.insert(std::make_pair("ttf", "font/ttf"));
    this->_ex_type.insert(std::make_pair("ttl", "text/turtle"));
    this->_ex_type.insert(std::make_pair("txt", "text/plain"));
    this->_ex_type.insert(std::make_pair("u8dsn", "message/global-delivery-status"));
    this->_ex_type.insert(std::make_pair("u8hdr", "message/global-headers"));
    this->_ex_type.insert(std::make_pair("u8mdn", "message/global-disposition-notification"));
    this->_ex_type.insert(std::make_pair("u8msg", "message/global"));
    this->_ex_type.insert(std::make_pair("uri", "text/uri-list"));
    this->_ex_type.insert(std::make_pair("uris", "text/uri-list"));
    this->_ex_type.insert(std::make_pair("urls", "text/uri-list"));
    this->_ex_type.insert(std::make_pair("vcard", "text/vcard"));
    this->_ex_type.insert(std::make_pair("vrml", "model/vrml"));
    this->_ex_type.insert(std::make_pair("vtt", "text/vtt"));
    this->_ex_type.insert(std::make_pair("vxml", "application/voicexml+xml"));
    this->_ex_type.insert(std::make_pair("war", "application/java-archive"));
    this->_ex_type.insert(std::make_pair("wasm", "application/wasm"));
    this->_ex_type.insert(std::make_pair("wav", "audio/wav"));
    this->_ex_type.insert(std::make_pair("weba", "audio/webm"));
    this->_ex_type.insert(std::make_pair("webm", "video/webm"));
    this->_ex_type.insert(std::make_pair("webmanifest", "application/manifest+json"));
    this->_ex_type.insert(std::make_pair("webp", "image/webp"));
    this->_ex_type.insert(std::make_pair("wgt", "application/widget"));
    this->_ex_type.insert(std::make_pair("wmf", "image/wmf"));
    this->_ex_type.insert(std::make_pair("woff", "font/woff"));
    this->_ex_type.insert(std::make_pair("woff2", "font/woff2"));
    this->_ex_type.insert(std::make_pair("wrl", "model/vrml"));
    this->_ex_type.insert(std::make_pair("wsdl", "application/wsdl+xml"));
    this->_ex_type.insert(std::make_pair("wspolicy", "application/wspolicy+xml"));
    this->_ex_type.insert(std::make_pair("x3d", "model/x3d+xml"));
    this->_ex_type.insert(std::make_pair("x3db", "model/x3d+binary"));
    this->_ex_type.insert(std::make_pair("x3dbz", "model/x3d+binary"));
    this->_ex_type.insert(std::make_pair("x3dv", "model/x3d+vrml"));
    this->_ex_type.insert(std::make_pair("x3dvz", "model/x3d+vrml"));
    this->_ex_type.insert(std::make_pair("x3dz", "model/x3d+xml"));
    this->_ex_type.insert(std::make_pair("xaml", "application/xaml+xml"));
    this->_ex_type.insert(std::make_pair("xdf", "application/xcap-diff+xml"));
    this->_ex_type.insert(std::make_pair("xdssc", "application/dssc+xml"));
    this->_ex_type.insert(std::make_pair("xenc", "application/xenc+xml"));
    this->_ex_type.insert(std::make_pair("xer", "application/patch-ops-error+xml"));
    this->_ex_type.insert(std::make_pair("xht", "application/xhtml+xml"));
    this->_ex_type.insert(std::make_pair("xhtml", "application/xhtml+xml"));
    this->_ex_type.insert(std::make_pair("xhvml", "application/xv+xml"));
    this->_ex_type.insert(std::make_pair("xm", "audio/xm"));
    this->_ex_type.insert(std::make_pair("xml", "application/xml"));
    this->_ex_type.insert(std::make_pair("xop", "application/xop+xml"));
    this->_ex_type.insert(std::make_pair("xpl", "application/xproc+xml"));
    this->_ex_type.insert(std::make_pair("xsd", "application/xml"));
    this->_ex_type.insert(std::make_pair("xsl", "application/xml"));
    this->_ex_type.insert(std::make_pair("xslt", "application/xslt+xml"));
    this->_ex_type.insert(std::make_pair("xspf", "application/xspf+xml"));
    this->_ex_type.insert(std::make_pair("xvm", "application/xv+xml"));
    this->_ex_type.insert(std::make_pair("xvml", "application/xv+xml"));
    this->_ex_type.insert(std::make_pair("yaml", "text/yaml"));
    this->_ex_type.insert(std::make_pair("yang", "application/yang"));
    this->_ex_type.insert(std::make_pair("yin", "application/yin+xml"));
    this->_ex_type.insert(std::make_pair("yml", "text/yaml"));
    this->_ex_type.insert(std::make_pair("zip", "application/zip"));
}

std::string MimeTypes::get_extention(std::string type){
     std::map<std::string , std::string>::iterator it = this->_ex_type.begin();
     while (it != this->_ex_type.end()){
        if (it->second == type)
            return "." + it->first;
        it++;
    }
    return "";
}

std::string MimeTypes::get_type(std::string path){
    size_t pos = path.rfind('.');
    if (pos != std::string::npos){
        std::map<std::string , std::string>::iterator it;
        it = this->_ex_type.begin();
        std::string last_dot = path.substr(pos + 1, path.size() - pos);
        while (it != this->_ex_type.end()){
            if (it->first == last_dot){
                std::string str = "Content-Type: ";
                str += it->second;
                return str + "\r\n";
            }
            it++;
        }
    }
    return "Content-Type: application/octet-stream\r\n";
}