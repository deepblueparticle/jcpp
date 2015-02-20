#include "jcpp/lang/JCharacterData02.h"
#include <cassert>

namespace jcpp {
	namespace lang {

	jint JCharacterData02::getProperties(jint ch) {
		jchar offset = (jchar)ch;
		jint props = A->getInt(Y->getChar(X->getChar(offset>>5)|((offset>>1)&0xF))|(offset&0x1));
		return props;
	}

	jint JCharacterData02::getPropertiesEx(jint ch) {
		jchar offset = (jchar)ch;
		jint props = B->getChar((jint)Y->getChar(X->getChar(offset>>5)|((offset>>1)&0xF))|(offset&0x1));
		return props;
	}

	jbool JCharacterData02::isOtherLowercase(jint ch) {
		jint props = getPropertiesEx(ch);
		return (props & 0x0001) != 0;
	}

	jbool JCharacterData02::isOtherUppercase(jint ch) {
		jint props = getPropertiesEx(ch);
		return (props & 0x0002) != 0;
	}

	jbool JCharacterData02::isOtherAlphabetic(jint ch) {
		jint props = getPropertiesEx(ch);
		return (props & 0x0004) != 0;
	}

	jbool JCharacterData02::isIdeographic(jint ch) {
		jint props = getPropertiesEx(ch);
		return (props & 0x0010) != 0;
	}

	jint JCharacterData02::getType(jint ch) {
		jint props = getProperties(ch);
		return (props & 0x1F);
	}

	jbool JCharacterData02::isJavaIdentifierStart(jint ch) {
		jint props = getProperties(ch);
		return ((props & 0x00007000) >= 0x00005000);
	}

	jbool JCharacterData02::isJavaIdentifierPart(jint ch) {
		jint props = getProperties(ch);
		return ((props & 0x00003000) != 0);
	}

	jbool JCharacterData02::isUnicodeIdentifierStart(jint ch) {
		jint props = getProperties(ch);
		return ((props & 0x00007000) == 0x00007000);
	}

	jbool JCharacterData02::isUnicodeIdentifierPart(jint ch) {
		jint props = getProperties(ch);
		return ((props & 0x00001000) != 0);
	}

	jbool JCharacterData02::isIdentifierIgnorable(jint ch) {
		jint props = getProperties(ch);
		return ((props & 0x00007000) == 0x00001000);
	}

	jint JCharacterData02::toLowerCase(jint ch) {
		jint mapChar = ch;
		jint val = getProperties(ch);

		if ((val & 0x00020000) != 0) {
			jint offset = val << 5 >> (5+18);
			mapChar = ch + offset;
		}
		return mapChar;
	}

	jint JCharacterData02::toUpperCase(jint ch) {
		jint mapChar = ch;
		jint val = getProperties(ch);

		if ((val & 0x00010000) != 0) {
			jint offset = val << 5 >> (5+18);
			mapChar = ch - offset;
		}
		return mapChar;
	}

	jint JCharacterData02::toTitleCase(jint ch) {
		jint mapChar = ch;
		jint val = getProperties(ch);

		if ((val & 0x00008000) != 0) {
			if ((val & 0x00010000) == 0) {
				mapChar = ch + 1;
			}
			else if ((val & 0x00020000) == 0) {
				mapChar = ch - 1;
			}
		}
		else if ((val & 0x00010000) != 0) {
			mapChar = toUpperCase(ch);
		}
		return mapChar;
	}

	jint JCharacterData02::digit(jint ch, jint radix) {
		jint value = -1;
		if (radix >= JCharacter::MIN_RADIX && radix <= JCharacter::MAX_RADIX) {
			jint val = getProperties(ch);
			jint kind = val & 0x1F;
			if (kind == JCharacter::DECIMAL_DIGIT_NUMBER) {
				value = ch + ((val & 0x3E0) >> 5) & 0x1F;
			}
			else if ((val & 0xC00) == 0x00000C00) {
				value = (ch + ((val & 0x3E0) >> 5) & 0x1F) + 10;
			}
		}
		return (value < radix) ? value : -1;
	}

	jint JCharacterData02::getNumericValue(jint ch) {
		jint val = getProperties(ch);
		jint retval = -1;

		switch (val & 0xC00) {
		default:
		case (0x00000000):         // not numeric
			retval = -1;
			break;
		case (0x00000400):              // simple numeric
			retval = ch + ((val & 0x3E0) >> 5) & 0x1F;
			break;
		case (0x00000800)      :       // "strange" numeric
			retval = -2;
			break;
		case (0x00000C00):           // Java supradecimal
			retval = (ch + ((val & 0x3E0) >> 5) & 0x1F) + 10;
			break;
		}
		return retval;
	}

	jbool JCharacterData02::isWhitespace(jint ch) {
		return (getProperties(ch) & 0x00007000) == 0x00004000;
	}

	jbyte JCharacterData02::getDirectionality(jint ch) {
		jint val = getProperties(ch);
		jbyte directionality = (jbyte)((val & 0x78000000) >> 27);
		if (directionality == 0xF ) {
			directionality = JCharacter::DIRECTIONALITY_UNDEFINED;
		}
		return directionality;
	}

	jbool JCharacterData02::isMirrored(jint ch) {
		return (getProperties(ch) & 0x80000000) != 0;
	}

	JCharacterData* JCharacterData02::instance = new JCharacterData02();

	JCharacterData02::JCharacterData02() : JCharacterData(getClazz()) {
	}

	JPrimitiveCharArray* JCharacterData02::initX() {
		jchar Xbuf[] = {0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0020, 0040, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0060, 0000, 0000, 0000, 0000, 0000, 0000, 0100, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0000, 0000, 0000, 0000, 0120, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0100, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040, 0040,
				0040, 0040, 0040, 0040, 0040, 0040, 0040, 0x040};
		return new JPrimitiveCharArray(2048, Xbuf);
	}

	JString* JCharacterData02::initA_DATA() {
		jchar A_DATAbuf[] = {0000, 0x7005, 0000, 0x7005, 0000, 0x7005, 0x7800, 0000, 0x7800, 0000, 0x7800, 0000, 0000, 0x7725, 0000, 0x7005};
		return new JString(new JPrimitiveCharArray(16, A_DATAbuf));
	}

	JPrimitiveCharArray* JCharacterData02::initY() {
		jchar Ybuf[] = {0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0002, 0004, 0004, 0004, 0004, 0004, 0004, 0004, 0004, 0004, 0004, 0004, 0004,
				0004, 0004, 0004, 0004, 0004, 0004, 0004, 0004, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0002, 0004,
				0004, 0004, 0004, 0004, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0004,
				0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0006, 0000, 0000, 0000, 0000, 0000, 0000, 0x000};
		return new JPrimitiveCharArray(96, Ybuf);
	}

	JPrimitiveCharArray* JCharacterData02::initB() {
		jchar Bbuf[] = {0020, 0020, 0020, 0000, 0000, 0000, 0020, 0x020};
		return new JPrimitiveCharArray(8, Bbuf);
	}

	JPrimitiveIntArray* JCharacterData02::initA() {
		JPrimitiveCharArray* data = A_DATA->toCharArray();
		assert(data->size() == (8 * 2));
		A = new JPrimitiveIntArray(8);
		jint i = 0, j = 0;
		while (i < (8 * 2)) {
			jint entry = data->getChar(i++) << 16;
			A->setInt(j++, entry | data->getChar(i++));
		}
		return A;
	}

	JString* JCharacterData02::A_DATA = JCharacterData02::initA_DATA();

	JPrimitiveCharArray* JCharacterData02::X = JCharacterData02::initX();

	JPrimitiveCharArray* JCharacterData02::Y = JCharacterData02::initY();

	JPrimitiveCharArray* JCharacterData02::B = JCharacterData02::initB();

	JPrimitiveIntArray* JCharacterData02::A = JCharacterData02::initA();

	}
}

