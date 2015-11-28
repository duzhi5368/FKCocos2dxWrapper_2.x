//-------------------------------------------------------------------------
#include "../Include/FKCW_Base_DecodeHtmlEntitiesUTF8.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
//-------------------------------------------------------------------------
#define UNICODE_MAX 0x10FFFFul
//-------------------------------------------------------------------------
static const char *const NAMED_ENTITIES[][2] = {
	{ "AElig;", "?" },
	{ "Aacute;", "?" },
	{ "Acirc;", "?" },
	{ "Agrave;", "?" },
	{ "Alpha;", "¦¡" },
	{ "Aring;", "?" },
	{ "Atilde;", "?" },
	{ "Auml;", "?" },
	{ "Beta;", "¦¢" },
	{ "Ccedil;", "?" },
	{ "Chi;", "¦¶" },
	{ "Dagger;", "?" },
	{ "Delta;", "¦¤" },
	{ "ETH;", "?" },
	{ "Eacute;", "?" },
	{ "Ecirc;", "?" },
	{ "Egrave;", "?" },
	{ "Epsilon;", "¦¥" },
	{ "Eta;", "¦§" },
	{ "Euml;", "?" },
	{ "Gamma;", "¦£" },
	{ "Iacute;", "?" },
	{ "Icirc;", "?" },
	{ "Igrave;", "?" },
	{ "Iota;", "¦©" },
	{ "Iuml;", "?" },
	{ "Kappa;", "¦ª" },
	{ "Lambda;", "¦«" },
	{ "Mu;", "¦¬" },
	{ "Ntilde;", "?" },
	{ "Nu;", "¦­" },
	{ "OElig;", "?" },
	{ "Oacute;", "?" },
	{ "Ocirc;", "?" },
	{ "Ograve;", "?" },
	{ "Omega;", "¦¸" },
	{ "Omicron;", "¦¯" },
	{ "Oslash;", "?" },
	{ "Otilde;", "?" },
	{ "Ouml;", "?" },
	{ "Phi;", "¦µ" },
	{ "Pi;", "¦°" },
	{ "Prime;", "¡å" },
	{ "Psi;", "¦·" },
	{ "Rho;", "¦±" },
	{ "Scaron;", "?" },
	{ "Sigma;", "¦²" },
	{ "THORN;", "?" },
	{ "Tau;", "¦³" },
	{ "Theta;", "¦¨" },
	{ "Uacute;", "?" },
	{ "Ucirc;", "?" },
	{ "Ugrave;", "?" },
	{ "Upsilon;", "¦´" },
	{ "Uuml;", "?" },
	{ "Xi;", "¦®" },
	{ "Yacute;", "?" },
	{ "Yuml;", "?" },
	{ "Zeta;", "¦¦" },
	{ "aacute;", "¨¢" },
	{ "acirc;", "?" },
	{ "acute;", "?" },
	{ "aelig;", "?" },
	{ "agrave;", "¨¤" },
	{ "alefsym;", "?" },
	{ "alpha;", "¦Á" },
	{ "amp;", "&" },
	{ "and;", "¡Ä" },
	{ "ang;", "¡Ï" },
	{ "apos;", "'" },
	{ "aring;", "?" },
	{ "asymp;", "¡Ö" },
	{ "atilde;", "?" },
	{ "auml;", "?" },
	{ "bdquo;", "?" },
	{ "beta;", "¦Â" },
	{ "brvbar;", "?" },
	{ "bull;", "?" },
	{ "cap;", "¡É" },
	{ "ccedil;", "?" },
	{ "cedil;", "?" },
	{ "cent;", "?" },
	{ "chi;", "¦Ö" },
	{ "circ;", "?" },
	{ "clubs;", "?" },
	{ "cong;", "?" },
	{ "copy;", "?" },
	{ "crarr;", "?" },
	{ "cup;", "¡È" },
	{ "curren;", "¡è" },
	{ "dArr;", "?" },
	{ "dagger;", "?" },
	{ "darr;", "¡ý" },
	{ "deg;", "¡ã" },
	{ "delta;", "¦Ä" },
	{ "diams;", "?" },
	{ "divide;", "¡Â" },
	{ "eacute;", "¨¦" },
	{ "ecirc;", "¨º" },
	{ "egrave;", "¨¨" },
	{ "empty;", "?" },
	{ "emsp;", "?" },
	{ "ensp;", "?" },
	{ "epsilon;", "¦Å" },
	{ "equiv;", "¡Ô" },
	{ "eta;", "¦Ç" },
	{ "eth;", "?" },
	{ "euml;", "?" },
	{ "euro;", "€" },
	{ "exist;", "?" },
	{ "fnof;", "?" },
	{ "forall;", "?" },
	{ "frac12;", "?" },
	{ "frac14;", "?" },
	{ "frac34;", "?" },
	{ "frasl;", "?" },
	{ "gamma;", "¦Ã" },
	{ "ge;", "¡Ý" },
	{ "gt;", ">" },
	{ "hArr;", "?" },
	{ "harr;", "?" },
	{ "hearts;", "?" },
	{ "hellip;", "¡­" },
	{ "iacute;", "¨ª" },
	{ "icirc;", "?" },
	{ "iexcl;", "?" },
	{ "igrave;", "¨¬" },
	{ "image;", "?" },
	{ "infin;", "¡Þ" },
	{ "int;", "¡Ò" },
	{ "iota;", "¦É" },
	{ "iquest;", "?" },
	{ "isin;", "¡Ê" },
	{ "iuml;", "?" },
	{ "kappa;", "¦Ê" },
	{ "lArr;", "?" },
	{ "lambda;", "¦Ë" },
	{ "lang;", "¡´" },
	{ "laquo;", "?" },
	{ "larr;", "¡û" },
	{ "lceil;", "?" },
	{ "ldquo;", "¡°" },
	{ "le;", "¡Ü" },
	{ "lfloor;", "?" },
	{ "lowast;", "?" },
	{ "loz;", "?" },
	{ "lrm;", "\xE2\x80\x8E" },
	{ "lsaquo;", "?" },
	{ "lsquo;", "¡®" },
	{ "lt;", "<" },
	{ "macr;", "?" },
	{ "mdash;", "¡ª" },
	{ "micro;", "?" },
	{ "middot;", "¡¤" },
	{ "minus;", "?" },
	{ "mu;", "¦Ì" },
	{ "nabla;", "?" },
	{ "nbsp;", "?" },
	{ "ndash;", "¨C" },
	{ "ne;", "¡Ù" },
	{ "ni;", "?" },
	{ "not;", "?" },
	{ "notin;", "?" },
	{ "nsub;", "?" },
	{ "ntilde;", "?" },
	{ "nu;", "¦Í" },
	{ "oacute;", "¨®" },
	{ "ocirc;", "?" },
	{ "oelig;", "?" },
	{ "ograve;", "¨°" },
	{ "oline;", "?" },
	{ "omega;", "¦Ø" },
	{ "omicron;", "¦Ï" },
	{ "oplus;", "¨’" },
	{ "or;", "¡Å" },
	{ "ordf;", "?" },
	{ "ordm;", "?" },
	{ "oslash;", "?" },
	{ "otilde;", "?" },
	{ "otimes;", "?" },
	{ "ouml;", "?" },
	{ "para;", "?" },
	{ "part;", "?" },
	{ "permil;", "¡ë" },
	{ "perp;", "¡Í" },
	{ "phi;", "¦Õ" },
	{ "pi;", "¦Ð" },
	{ "piv;", "?" },
	{ "plusmn;", "¡À" },
	{ "pound;", "?" },
	{ "prime;", "¡ä" },
	{ "prod;", "¡Ç" },
	{ "prop;", "¡Ø" },
	{ "psi;", "¦×" },
	{ "quot;", "\"" },
	{ "rArr;", "?" },
	{ "radic;", "¡Ì" },
	{ "rang;", "¡µ" },
	{ "raquo;", "?" },
	{ "rarr;", "¡ú" },
	{ "rceil;", "?" },
	{ "rdquo;", "¡±" },
	{ "real;", "?" },
	{ "reg;", "?" },
	{ "rfloor;", "?" },
	{ "rho;", "¦Ñ" },
	{ "rlm;", "\xE2\x80\x8F" },
	{ "rsaquo;", "?" },
	{ "rsquo;", "¡¯" },
	{ "sbquo;", "?" },
	{ "scaron;", "?" },
	{ "sdot;", "?" },
	{ "sect;", "¡ì" },
	{ "shy;", "\xC2\xAD" },
	{ "sigma;", "¦Ò" },
	{ "sigmaf;", "?" },
	{ "sim;", "?" },
	{ "spades;", "?" },
	{ "sub;", "?" },
	{ "sube;", "?" },
	{ "sum;", "¡Æ" },
	{ "sup;", "?" },
	{ "sup1;", "?" },
	{ "sup2;", "?" },
	{ "sup3;", "?" },
	{ "supe;", "?" },
	{ "szlig;", "?" },
	{ "tau;", "¦Ó" },
	{ "there4;", "¡à" },
	{ "theta;", "¦È" },
	{ "thetasym;", "?" },
	{ "thinsp;", "?" },
	{ "thorn;", "?" },
	{ "tilde;", "?" },
	{ "times;", "¡Á" },
	{ "trade;", "?" },
	{ "uArr;", "?" },
	{ "uacute;", "¨²" },
	{ "uarr;", "¡ü" },
	{ "ucirc;", "?" },
	{ "ugrave;", "¨´" },
	{ "uml;", "¡§" },
	{ "upsih;", "?" },
	{ "upsilon;", "¦Ô" },
	{ "uuml;", "¨¹" },
	{ "weierp;", "?" },
	{ "xi;", "¦Î" },
	{ "yacute;", "?" },
	{ "yen;", "?" },
	{ "yuml;", "?" },
	{ "zeta;", "¦Æ" },
	{ "zwj;", "\xE2\x80\x8D" },
	{ "zwnj;", "\xE2\x80\x8C" }
};

static int cmp(const void *key, const void *value)
{
	return strncmp((const char *)key, *(const char **)value,
		strlen(*(const char **)value));
}

static const char *get_named_entity(const char *name)
{
	const char *const *entity = (const char *const *)bsearch(name,
		NAMED_ENTITIES, sizeof NAMED_ENTITIES / sizeof *NAMED_ENTITIES,
		sizeof *NAMED_ENTITIES, cmp);

	return entity ? entity[1] : NULL;
}

static size_t putc_utf8(unsigned long cp, char *buffer)
{
	unsigned char *bytes = (unsigned char *)buffer;

	if(cp <= 0x007Ful)
	{
		bytes[0] = (unsigned char)cp;
		return 1;
	}

	if(cp <= 0x07FFul)
	{
		bytes[1] = (unsigned char)((2 << 6) | (cp & 0x3F));
		bytes[0] = (unsigned char)((6 << 5) | (cp >> 6));
		return 2;
	}

	if(cp <= 0xFFFFul)
	{
		bytes[2] = (unsigned char)(( 2 << 6) | ( cp       & 0x3F));
		bytes[1] = (unsigned char)(( 2 << 6) | ((cp >> 6) & 0x3F));
		bytes[0] = (unsigned char)((14 << 4) |  (cp >> 12));
		return 3;
	}

	if(cp <= 0x10FFFFul)
	{
		bytes[3] = (unsigned char)(( 2 << 6) | ( cp        & 0x3F));
		bytes[2] = (unsigned char)(( 2 << 6) | ((cp >>  6) & 0x3F));
		bytes[1] = (unsigned char)(( 2 << 6) | ((cp >> 12) & 0x3F));
		bytes[0] = (unsigned char)((30 << 3) |  (cp >> 18));
		return 4;
	}

	return 0;
}

static bool parse_entity(
	const char *current, char **to, const char **from)
{
	const char *end = strchr(current, ';');
	if(!end) return 0;

	if(current[1] == '#')
	{
		char *tail = NULL;
		int errno_save = errno;
		bool hex = current[2] == 'x' || current[2] == 'X';

		errno = 0;
		unsigned long cp = strtoul(
			current + (hex ? 3 : 2), &tail, hex ? 16 : 10);

		bool fail = errno || tail != end || cp > UNICODE_MAX;
		errno = errno_save;
		if(fail) return 0;

		*to += putc_utf8(cp, *to);
		*from = end + 1;

		return 1;
	}
	else
	{
		const char *entity = get_named_entity(&current[1]);
		if(!entity) return 0;

		size_t len = strlen(entity);
		memcpy(*to, entity, len);

		*to += len;
		*from = end + 1;

		return 1;
	}
}

size_t decode_html_entities_utf8(char *dest, const char *src)
{
	if(!src) src = dest;

	char *to = dest;
	const char *from = src;
	const char *current = strchr(from, '&');
	while(current) {
		memmove(to, from, (size_t)(current - from));
		to += current - from;
		
		if(!parse_entity(current, &to, &from)) {
			from = current;
			*to++ = *from++;
		}
		
		current = strchr(from, '&');
	}

	size_t remaining = strlen(from);

	memmove(to, from, remaining);
	to += remaining;
	*to = 0;

	return (size_t)(to - dest);
}
//-------------------------------------------------------------------------