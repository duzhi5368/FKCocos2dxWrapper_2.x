//-------------------------------------------------------------------------
#include "../Include/FKCW_TMX_Loader.h"
#include "../Include/FKCW_TMX_MapInfo.h"
#include "../Include/FKCW_TMX_TileSetInfo.h"
#include "../Include/FKCW_TMX_Object.h"
#include "../Include/FKCW_TMX_ObjectGroup.h"
#include "../Include/FKCW_TMX_LayerInfo.h"
#include "../support/zip_support/ZipUtils.h"
//-------------------------------------------------------------------------
FKCW_TMX_Loader::FKCW_TMX_Loader() :
	m_compressed(false),
	m_lastGid(-1) 
{
		m_map = FKCW_TMX_MapInfo::create();
		CC_SAFE_RETAIN(m_map);
}
//-------------------------------------------------------------------------
FKCW_TMX_Loader::~FKCW_TMX_Loader() 
{
	CC_SAFE_RELEASE(m_map);
}
//-------------------------------------------------------------------------
FKCW_TMX_Loader* FKCW_TMX_Loader::create() 
{
	FKCW_TMX_Loader* l = new FKCW_TMX_Loader();
	return (FKCW_TMX_Loader*)l->autorelease();
}
//-------------------------------------------------------------------------
FKCW_TMX_Loader::TMXTag FKCW_TMX_Loader::getTag(const char* name)
{
	if(!strcmp(name, "map"))
		return MAP;
	else if(!strcmp(name, "tileset"))
		return TILESET;
	else if(!strcmp(name, "tile"))
		return TILE;
	else if(!strcmp(name, "image"))
		return IMAGE;
	else if(!strcmp(name, "layer"))
		return LAYER;
	else if(!strcmp(name, "data"))
		return DATA;
	else if(!strcmp(name, "properties"))
		return PROPERTIES;
	else if(!strcmp(name, "property"))
		return PROPERTY;
	else if(!strcmp(name, "objectgroup"))
		return OBJECTGROUP;
	else if(!strcmp(name, "object"))
		return OBJECT;
	else if(!strcmp(name, "polyline"))
		return POLYLINE;
	else if(!strcmp(name, "polygon"))
		return POLYGON;
	else
		return UNKNOWN;
}
//-------------------------------------------------------------------------
FKCW_TMX_Loader::TMXAttr FKCW_TMX_Loader::getAttr(const char* name)
{
	if(!strcmp(name, "version"))
		return ATTR_VERSION;
	else if(!strcmp(name, "orientation"))
		return ATTR_ORIENTATION;
	else if(!strcmp(name, "width"))
		return ATTR_WIDTH;
	else if(!strcmp(name, "height"))
		return ATTR_HEIGHT;
	else if(!strcmp(name, "tilewidth"))
		return ATTR_TILEWIDTH;
	else if(!strcmp(name, "tileheight"))
		return ATTR_TILEHEIGHT;
	else if(!strcmp(name, "name"))
		return ATTR_NAME;
	else if(!strcmp(name, "firstgid"))
		return ATTR_FIRSTGID;
	else if(!strcmp(name, "spacing"))
		return ATTR_SPACING;
	else if(!strcmp(name, "margin"))
		return ATTR_MARGIN;
	else if(!strcmp(name, "visible"))
		return ATTR_VISIBLE;
	else if(!strcmp(name, "opacity"))
		return ATTR_OPACITY;
	else if(!strcmp(name, "x"))
		return ATTR_X;
	else if(!strcmp(name, "y"))
		return ATTR_Y;
	else if(!strcmp(name, "type"))
		return ATTR_TYPE;
	else if(!strcmp(name, "color"))
		return ATTR_COLOR;
	else if(!strcmp(name, "points"))
		return ATTR_POINTS;
	else
		return ATTR_UNKNOWN;
}
//-------------------------------------------------------------------------
void FKCW_TMX_Loader::pushTag(TMXTag tag) 
{
	m_tags.push_back(tag);
}
//-------------------------------------------------------------------------
void FKCW_TMX_Loader::popTag() 
{
	m_tags.pop_back();
}
//-------------------------------------------------------------------------
FKCW_TMX_Loader::TMXTag FKCW_TMX_Loader::topTag() 
{
	return *m_tags.rbegin();
}
//-------------------------------------------------------------------------
FKCW_TMX_Loader::TMXTag FKCW_TMX_Loader::parentTag()
{
	return *(m_tags.rbegin() + 1);
}
//-------------------------------------------------------------------------
FKCW_TMX_Loader::TMXTag FKCW_TMX_Loader::grandpaTag() 
{
	return *(m_tags.rbegin() + 2);
}
//-------------------------------------------------------------------------
const char* FKCW_TMX_Loader::valueForKey(const char** atts, const char* name)
{
	for(int i = 0; atts[i] != NULL; i += 2)
	{
		if(!strcmp(atts[i], name))
			return atts[i + 1];
	}
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_TMX_MapInfo* FKCW_TMX_Loader::load(const string& tmxFile) 
{
	// 获取路径
	m_tmxDir = FKCW_Base_Utils::DeleteLastPathOfComponment( tmxFile );

	// 开始处理
	unsigned long size;
	unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(tmxFile.c_str(), "rb", &size);
	bool success = load((const char*)data, (int)size);
	free(data);
	return success ? m_map : NULL;
}
//-------------------------------------------------------------------------
FKCW_TMX_MapInfo* FKCW_TMX_Loader::load(const char* tmxData, int length, const string& resourcePath)
{
	// 获取路径
	m_tmxDir = resourcePath;

	// 开始加载
	if(load(tmxData, length)) {
		return m_map;
	} else {
		return NULL;
	}
}
//-------------------------------------------------------------------------
bool FKCW_TMX_Loader::load(const char* data, int length)
{
	CCSAXParser parser;
	if(!parser.init("UTF-8")) 
	{
		return false;
	}
	parser.setDelegator(this);
	return parser.parse(data, length);
}
//-------------------------------------------------------------------------
void FKCW_TMX_Loader::startElement(void* ctx, const char* name, const char** atts) 
{
	// 设置标签
	pushTag(getTag((const char*)name));

	float fValue = 0.0f;
	int nValue = 0;
	switch(topTag()) {
	case MAP:
		for(int i = 0; atts[i] != NULL; i += 2) 
		{
			// get attr and value
			const char* key = (const char*)atts[i];
			const char* value = (const char*)atts[i + 1];
			TMXAttr attr = getAttr(key);

			// check attr
			switch(attr) {
			case ATTR_VERSION:
				if(strcmp("1.0", value)) {
					CCLOGERROR("Doesn't support TMX file whose version is not 1.0");
				}
				break;
			case ATTR_ORIENTATION:
				if(!strcmp(value, "orthogonal"))
					m_map->setOrientation(eTMXOrientationOrthogonal);
				else if(!strcmp(value, "isometric"))
					m_map->setOrientation(eTMXOrientationIsometric);
				else if(!strcmp(value, "hexagonal"))
					m_map->setOrientation(eTMXOrientationHexagonal);
				else {
					CCLOGERROR("Unsupported orientation: %s", value);
				}
				break;
			case ATTR_WIDTH:
				m_map->setMapWidth(atoi(value));
				break;
			case ATTR_HEIGHT:
				m_map->setMapHeight(atoi(value));
				break;
			case ATTR_TILEWIDTH:
				fValue = static_cast<float>(atof(value) / CC_CONTENT_SCALE_FACTOR());
				m_map->setTileWidth(fValue);
				break;
			case ATTR_TILEHEIGHT:
				fValue = static_cast<float>(atof(value) / CC_CONTENT_SCALE_FACTOR());
				m_map->setTileHeight(fValue);
				break;
			default:
				break;
			}
		}
		break;
	case TILESET:
		{
			const char* externalFile = valueForKey((const char**)atts, "source");
			if(externalFile == NULL) 
			{
				FKCW_TMX_TileSetInfo* tileset = FKCW_TMX_TileSetInfo::create();
				m_map->getTileSets().addObject(tileset);

				for(int i = 0; atts[i] != NULL; i += 2) 
				{
					// get attr and value
					const char* key = (const char*)atts[i];
					const char* value = (const char*)atts[i + 1];
					TMXAttr attr = getAttr(key);
					fValue = static_cast<float>(atof(value) / CC_CONTENT_SCALE_FACTOR());

					// check attr
					switch(attr) 
					{
					case ATTR_NAME:
						tileset->setName(value);
						break;
					case ATTR_FIRSTGID:
						tileset->setFirstGid(atoi(value));
						break;
					case ATTR_SPACING:
						
						tileset->setSpacing(fValue);
						break;
					case ATTR_MARGIN:
						tileset->setMargin(fValue);
						break;
					case ATTR_TILEWIDTH:
						tileset->setTileWidth(fValue);
						break;
					case ATTR_TILEHEIGHT:
						tileset->setTileHeight(fValue);
						break;
					default:
						break;
					}
				}
			} 
			else 
			{
				string externalFilePath = FKCW_Base_Utils::AppendLastPathOfComponment(m_tmxDir, externalFile);
				unsigned long size;
				unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(externalFilePath.c_str(), "rb", &size);
				bool success = load((const char*)data, (int)size);
				free(data);

				// firstgid is not written in external tileset, so we must read it from current tmx
				if(success) 
				{
					FKCW_TMX_TileSetInfo* tileset = (FKCW_TMX_TileSetInfo*)m_map->getTileSets().lastObject();
					for(int i = 0; atts[i] != NULL; i += 2) 
					{
						// get attr and value
						const char* key = (const char*)atts[i];
						const char* value = (const char*)atts[i + 1];
						TMXAttr attr = getAttr(key);

						// check attr
						switch(attr) 
						{
						case ATTR_FIRSTGID:
							tileset->setFirstGid(atoi(value));
							break;
						default:
							break;
						}
					}
				}
			}
			break;
		}
	case IMAGE:
		{
			string imageName = FKCW_Base_Utils::LastPathOfComponent(valueForKey((const char**)atts, "source"));
			string imagePath = FKCW_Base_Utils::AppendLastPathOfComponment(m_tmxDir, imageName);
			FKCW_TMX_TileSetInfo* tileset = (FKCW_TMX_TileSetInfo*)m_map->getTileSets().lastObject();
			tileset->setSourceImagePath(imagePath);
			break;
		}
	case TILE:
		{
			FKCW_TMX_TileSetInfo* tileset = (FKCW_TMX_TileSetInfo*)m_map->getTileSets().lastObject();
			m_lastGid = tileset->getFirstGid() + atoi(valueForKey((const char**)atts, "id"));
			break;
		}
	case PROPERTY:
		switch(grandpaTag()) 
		{
		case MAP:
			m_map->addProperty(valueForKey((const char**)atts, "name"), (const char*)valueForKey((const char**)atts, "value"));
			break;
		case TILE:
			m_map->addTileProperty(m_lastGid, valueForKey((const char**)atts, "name"), (const char*)valueForKey((const char**)atts, "value"));
			break;
		case LAYER:
			{
				FKCW_TMX_LayerInfo* layer = (FKCW_TMX_LayerInfo*)m_map->getLayers().lastObject();
				layer->addProperty(valueForKey((const char**)atts, "name"), valueForKey((const char**)atts, "value"));
				break;
			}
		case OBJECTGROUP:
			{
				FKCW_TMX_ObjectGroup* group = (FKCW_TMX_ObjectGroup*)m_map->getObjectGroups().lastObject();
				group->addProperty(valueForKey((const char**)atts, "name"), valueForKey((const char**)atts, "value"));
				break;
			}
		case OBJECT:
			{
				FKCW_TMX_ObjectGroup* group = (FKCW_TMX_ObjectGroup*)m_map->getObjectGroups().lastObject();
				FKCW_TMX_Object* object = group->getObjectAt(group->getObjectCount() - 1);
				object->addProperty(valueForKey((const char**)atts, "name"), valueForKey((const char**)atts, "value"));
				break;
			}
		default:
			break;
		}
		break;
	case LAYER:
		{
			FKCW_TMX_LayerInfo* layer = FKCW_TMX_LayerInfo::create();
			m_map->getLayers().addObject(layer);

			for(int i = 0; atts[i] != NULL; i += 2) 
			{
				// get attr and value
				const char* key = (const char*)atts[i];
				const char* value = (const char*)atts[i + 1];
				TMXAttr attr = getAttr(key);

				// check some optional attribute
				const char* visible = valueForKey((const char**)atts, "visible");
				if(visible == NULL)
					layer->setVisible(true);
				const char* alpha = valueForKey((const char**)atts, "opacity");
				if(alpha == NULL)
					layer->setAlpha(255);

				// check attr
				switch(attr) 
				{
				case ATTR_NAME:
					layer->setName(value);
					break;
				case ATTR_WIDTH:
					layer->setLayerWidth(atoi(value));
					break;
				case ATTR_HEIGHT:
					layer->setLayerHeight(atoi(value));
					break;
				case ATTR_VISIBLE:
					layer->setVisible(atoi(value) == 1);
					break;
				case ATTR_OPACITY:
					nValue = static_cast<int>(atof(value) * 255.f);
					layer->setAlpha(nValue);
					break;
				case ATTR_X:
					fValue = static_cast<float>(atof(value) / CC_CONTENT_SCALE_FACTOR());
					layer->setOffsetX(fValue);
					break;
				case ATTR_Y:
					fValue = static_cast<float>(atof(value) / CC_CONTENT_SCALE_FACTOR());
					layer->setOffsetY(fValue);
					break;
				default:
					break;
				}
			}
			break;
		}
	case DATA:
		{
			// check encoding
			const char* encoding = valueForKey((const char**)atts, "encoding");
			if(strcmp(encoding, "base64")) {
				CCLOGERROR("Unsupported layer data encoding: %s", encoding);
				break;
			}

			// check compression
			const char* compression = valueForKey((const char**)(atts), "compression");
			m_compressed = compression != NULL;
			if(compression != NULL && strcmp(compression, "gzip") && strcmp(compression, "zlib")) 
			{
				CCLOGERROR("Unsupported layer data compression: %s", compression);
				break;
			}

			break;
		}
	case OBJECTGROUP:
		{
			FKCW_TMX_ObjectGroup* group = FKCW_TMX_ObjectGroup::create();
			m_map->getObjectGroups().addObject(group);

			for(int i = 0; atts[i] != NULL; i += 2) 
			{
				// get attr and value
				const char* key = (const char*)atts[i];
				const char* value = (const char*)atts[i + 1];
				TMXAttr attr = getAttr(key);

				// check attr
				switch(attr) {
				case ATTR_NAME:
					group->setName(value);
					break;
				case ATTR_X:
					fValue = static_cast<float>(atof(value) / CC_CONTENT_SCALE_FACTOR());
					group->setOffsetX(fValue);
					break;
				case ATTR_Y:
					fValue = static_cast<float>(atof(value) / CC_CONTENT_SCALE_FACTOR());
					group->setOffsetY(fValue);
					break;
				case ATTR_COLOR:
					{
						int color;
						sscanf(value, "#%x", &color);
						color |= 0xff000000;
						group->setColor(color);
						break;
					}
				case ATTR_OPACITY:
					fValue = static_cast<float>(atof(value));
					group->setOpacity(fValue);
					break;
				default:
					break;
				}
			}

			break;
		}
	case OBJECT:
		{
			// create new object hash
			FKCW_TMX_ObjectGroup* group = (FKCW_TMX_ObjectGroup*)m_map->getObjectGroups().lastObject();
			FKCW_TMX_Object* object = group->newObject();

			// set object property
			for(int i = 0; atts[i] != NULL; i += 2) 
			{
				// get attr and value
				const char* key = (const char*)atts[i];
				const char* value = (const char*)atts[i + 1];
				TMXAttr attr = getAttr(key);

				// check attr
				switch(attr) 
				{
				case ATTR_NAME:
					object->setName(value);
					break;
				case ATTR_TYPE:
					object->setType(value);
					break;
				case ATTR_X:
					{
						float x = static_cast<float>(atof(value) / CC_CONTENT_SCALE_FACTOR());
						object->getPosition().x = x;
						break;
					}
				case ATTR_Y:
					{
						float y = static_cast<float>(atof(value) / CC_CONTENT_SCALE_FACTOR());
						object->getPosition().y = y;
						break;
					}
				case ATTR_WIDTH:
					{
						float w = static_cast<float>(atof(value) / CC_CONTENT_SCALE_FACTOR());
						object->getSize().width = w;
						break;
					}
				case ATTR_HEIGHT:
					{
						float h = static_cast<float>(atof(value) / CC_CONTENT_SCALE_FACTOR());
						object->getSize().height = h;
						break;
					}
				default:
					break;
				}
			}
			break;
		}
	case POLYGON:
	case POLYLINE:
		{
			switch(parentTag()) 
			{
			case OBJECT:
				{
					FKCW_TMX_ObjectGroup* group = (FKCW_TMX_ObjectGroup*)m_map->getObjectGroups().lastObject();
					FKCW_TMX_Object* object = group->getObjectAt(group->getObjectCount() - 1);

					// set type
					switch(topTag()) 
					{
					case POLYGON:
						object->setShape(FKCW_TMX_Object::eShape_POLYGON);
						break;
					case POLYLINE:
						object->setShape(FKCW_TMX_Object::eShape_POLYLINE);
						break;
					default:
						break;
					}

					// parse attributes
					for(int i = 0; atts[i] != NULL; i += 2) 
					{
						// get attr and value
						const char* key = (const char*)atts[i];
						const char* value = (const char*)atts[i + 1];
						TMXAttr attr = getAttr(key);

						// check attr
						switch(attr) 
						{
						case ATTR_POINTS:
							{
								// parse space character
								char* p = strtok((char*)value, " ");
								char* n = NULL;
								while(p) {
									n = strtok(NULL, " ");

									// parse point
									float x, y;
									sscanf(p, "%f,%f", &x, &y);
									object->getPoints().AddPoint(x / CC_CONTENT_SCALE_FACTOR(), y / CC_CONTENT_SCALE_FACTOR());

									// next token
									p = n;
								}

								break;
							}
						default:
							break;
						}
					}

					break;
				}
			default:
				break;
			}
			break;
		}
	default:
		break;
	}
}
//-------------------------------------------------------------------------
void FKCW_TMX_Loader::endElement(void* ctx, const char* name) 
{
	// pop
	popTag();
}
//-------------------------------------------------------------------------
void FKCW_TMX_Loader::textHandler(void* ctx, const char* s, int len) 
{
	// check tag
	switch(topTag()) 
	{
	case DATA:
		{
			// decode
			char* tmp = (char*)calloc(len + 1, sizeof(char));
			strncpy(tmp, (const char*)s, len);
			string tmpS = tmp;
			tmpS = FKCW_Base_Utils::Trim(tmpS);
			char* data = (char*)FKCW_Base_Base64::Decode(tmpS, &len);
			free(tmp);

			// decompress
			if(m_compressed) {
				unsigned char* deflated = NULL;
				FKCW_TMX_LayerInfo* layer = (FKCW_TMX_LayerInfo*)m_map->getLayers().lastObject();
				int sizeHint = (int)(layer->getLayerWidth() * layer->getLayerHeight() * sizeof(unsigned int));
				len = ZipUtils::ccInflateMemoryWithHint((unsigned char*)data, (unsigned int)len, &deflated, sizeHint);
				free(data);
				data = (char*)deflated;
			}

			// convert to int array
			if(len % 4 == 0) {
				FKCW_TMX_LayerInfo* layer = (FKCW_TMX_LayerInfo*)m_map->getLayers().lastObject();
				layer->setTiles((int*)malloc(len / 4 * sizeof(int)));
				int* tiles = layer->getTiles();
				for(int i = 0; i < len; i += 4) {
					tiles[i / 4] = ((data[i + 3] & 0xFF) << 24) | ((data[i + 2] & 0xFF) << 16) | ((data[i + 1] & 0xFF) << 8) | (data[i] & 0xFF);
				}
			}

			free(data);
			break;
		}
	default:
		break;
	}
}
//-------------------------------------------------------------------------