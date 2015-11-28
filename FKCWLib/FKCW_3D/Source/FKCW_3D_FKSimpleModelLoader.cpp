//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_FKSimpleModelLoader.h"
//-------------------------------------------------------------------------
static char tCharBuffer[1024]={0};
//-------------------------------------------------------------------------
static void fskipOneStr(FILE * fp,bool valueOnly)
{
	assert(fp);
	if(valueOnly==false){
		fscanf(fp,"%s",tCharBuffer);
	}
}
//-------------------------------------------------------------------------
static void fprintKey(FILE * fp,bool valueOnly,const string&key)
{
	assert(fp);
	if(valueOnly==false){
		fprintf(fp,"%s",key.c_str());
	}
}
//-------------------------------------------------------------------------
static FKCW_3D_Vector4 fscanVector4(FILE * fp)
{
	assert(fp);
	float arr[4];
	for(int i=0;i<4;i++){
		fscanf(fp,"%f",&arr[i]);
	}
	return FKCW_3D_Vector4(arr);
}
//-------------------------------------------------------------------------
static CCArray* getCCArrayByFileName(const char* pszFileName)
{
	string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszFileName);
	unsigned long len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &len);
	if( data == NULL )
	{
		cout<<"Open "<<fullpath<<" failed!"<<endl;
		assert( false );
		return NULL;
	}

	string strData = data;
	CCArray *pArrData = CCArray::createWithArray(&FKCW_Base_Utils::ComponentOfString(strData,'\n'));
	if (pArrData->count() == 0)
	{
		cout<<fullpath<<" have no data!"<<endl;
		assert(false);
		return NULL;
	}

	return pArrData;
}
//-------------------------------------------------------------------------
static FKCW_3D_Vector4 fVector4ByString(const char* pszArrayStr)
{
	if (pszArrayStr == NULL)
	{
		assert(false);
		return NULL;
	}

	FKCW_3D_Vector4 tV4;
	float a0,a1,a2,a3;
	CCString* pStr = NULL;
	CCArray *pArrData = CCArray::createWithArray(&FKCW_Base_Utils::ComponentOfString(pszArrayStr,' '));
	if (pArrData->count() != 4)
	{
		assert(false);
		return NULL;
	}
	pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(0));
	a0 = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));
	pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1));
	a1 = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));
	pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(2));
	a2 = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));
	pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(3));
	a3 = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));
	tV4.init(a0,a1,a2,a3);

	pArrData->removeAllObjects();
	pArrData = NULL;
	pStr = NULL;

	return tV4;
}
//-------------------------------------------------------------------------
static FKCW_3D_IDTriangle fIDTriangleByString(const char* pszArrayStr)
{
	FKCW_3D_IDTriangle idTriangle;
	idTriangle.init(0,0,0);
	if (pszArrayStr == NULL)
	{
		assert(false);
		return idTriangle;
	}
	int nID0,nID1,nID2;
	CCString* pStr = NULL;
	CCArray *pArrData = CCArray::createWithArray(&FKCW_Base_Utils::ComponentOfString(pszArrayStr,' '));
	if (pArrData->count() != 3)
	{
		assert(false);
		return idTriangle;
	}

	pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(0));
	nID0 = (FKCW_Base_Utils::AtoI(pStr->getCString()));
	pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1));
	nID1 = (FKCW_Base_Utils::AtoI(pStr->getCString()));
	pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(2));
	nID2 = (FKCW_Base_Utils::AtoI(pStr->getCString()));
	idTriangle.init(nID0,nID1,nID2);

	pArrData->removeAllObjects();
	pArrData = NULL;
	pStr = NULL;

	return idTriangle;
}
//-------------------------------------------------------------------------
static FKCW_3D_Vector2 fVector2ByString(const char* pszArrayStr)
{
	if (pszArrayStr == NULL)
	{
		assert(false);
		return NULL;
	}
	FKCW_3D_Vector2 tV2;
	float a0,a1;
	CCString* pStr = NULL;
	CCArray *pArrData = CCArray::createWithArray(&FKCW_Base_Utils::ComponentOfString(pszArrayStr,' '));
	if (pArrData->count() != 2)
	{
		assert(false);
		return NULL;
	}
	pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(0));
	a0 = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));
	pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1));
	a1 = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));
	tV2.init(a0,a1);

	pArrData->removeAllObjects();
	pArrData = NULL;
	pStr = NULL;

	return tV2;
}
//-------------------------------------------------------------------------
FKCW_3D_Actor* CreateActorByFKSimpleFile(string path)
{
	//----FKCWSkinActor.txt
	FKCW_3D_Actor*actor=new FKCW_3D_Actor();
	actor->autorelease();
	actor->init();
	FKCW_3D_Model*model=new FKCW_3D_Model();
	model->autorelease();
	model->init();
	int skinMeshCount;
	CCArray *pArrData = NULL;
	CCString* pStr = NULL;
	bool valueOnly;
	{
		
		pArrData = getCCArrayByFileName((path+"/FKCWSkinActor.txt").c_str());
		if (pArrData->count() < 5)
		{
			assert(false);
		}

		//------------------read file
		/*
		valueOnly
		0
		0.100000
		1
		2

		*/
		string str;
		pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(0));
		if (pStr == NULL)
		{
			assert(false);
		}

		if (pStr->compare("valueOnly"))
		{
			valueOnly=true;
		}else if(pStr->compare("notValueOnly")){
			valueOnly=false;
		}else{
			assert(false);
		}

		pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1));
		if (pStr == NULL)
		{
			assert(false);
		}
		str = pStr->getCString();
		int aniLayerIndex = FKCW_Base_Utils::AtoI(str);

		pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(2));
		if (pStr == NULL)
		{
			assert(false);
		}
		float interval = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));

		pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(3));
		if (pStr == NULL)
		{
			assert(false);
		}
		str = pStr->getCString();
		int t_nAniLayerInfo = FKCW_Base_Utils::AtoI(str);
		
		pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(4));
		if (pStr == NULL)
		{
			assert(false);
		}
		str = pStr->getCString();
		int t_nSkinMesh = FKCW_Base_Utils::AtoI(str);

		pArrData->removeAllObjects();
		pArrData = NULL;

		skinMeshCount = t_nSkinMesh;

	}

	//----m_skinMesh_0 ~ m_skinMesh_[skinMeshCount-1]
	for(int skinMeshIndex=0;skinMeshIndex<skinMeshCount;skinMeshIndex++){
		//----FKCWSkinMesh.txt
		FKCW_3D_Mesh* mesh=new FKCW_3D_Mesh();
		mesh->autorelease();
		mesh->init();
		int skinSubMeshCount;
		{
			string fullpath=CCFileUtils::sharedFileUtils()->fullPathForFilename((path+"/m_skinMesh_"+FKCW_Base_Utils::IntToString(skinMeshIndex)+"/FKCWSkinMesh.txt").c_str());
			////open file
			pArrData = getCCArrayByFileName(fullpath.c_str());
			if (pArrData->count() < 4)
			{
				assert(false);
			}
			//------------------read file
			//pArrData[0]
			pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(0));
			if (pStr == NULL)
			{
				assert(false);
			}
			int t_nSkinSubMesh = FKCW_Base_Utils::AtoI(pStr->getCString());
			if(!valueOnly)assert(string(tCharBuffer)=="m_skinSubMesh.size");

			//pArrData[1]
			pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1));
			if (pStr == NULL)
			{
				assert(false);
			}
			int t_nVertexDup = FKCW_Base_Utils::AtoI(pStr->getCString());
			if(!valueOnly)assert(string(tCharBuffer)=="m_vertexDupList.size");
			assert(t_nVertexDup==0);

			//pArrData[2]
			//RTmat 
			float m[16];
			pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(2));
			if (pStr == NULL)
			{
				assert(false);
			}
			CCArray *pArrDataRTmat = CCArray::createWithArray(&FKCW_Base_Utils::ComponentOfString(pStr->getCString(),' '));
			if (pArrDataRTmat->count() != 16)
			{
				assert(false);
			}
			if(!valueOnly)assert(string(tCharBuffer)=="RTmat");

			for(int j=0;j<16;j++){
				CCObject* pObject = pArrDataRTmat->objectAtIndex(j);
				pStr = dynamic_cast<CCString*>(pObject);
				m[j] = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));
			}
			FKCW_3D_Matrix4 t_RTmat(m);
			pArrDataRTmat->removeAllObjects();
			pArrDataRTmat = NULL;

			//pArrData[3]
			//Scale
			float t_scaleX,t_scaleY,t_scaleZ;
			pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(3));
			if (pStr == NULL)
			{
				assert(false);
			}
			CCArray *pArrDataScale = CCArray::createWithArray(&FKCW_Base_Utils::ComponentOfString(pStr->getCString(),' '));
			if (pArrDataScale->count() != 3)
			{
				assert(false);
			}
			if(!valueOnly)assert(string(tCharBuffer)=="scale");

			pStr = dynamic_cast<CCString*>(pArrDataScale->objectAtIndex(0));
			t_scaleX = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));
			pStr = dynamic_cast<CCString*>(pArrDataScale->objectAtIndex(1));
			t_scaleY = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));
			pStr = dynamic_cast<CCString*>(pArrDataScale->objectAtIndex(2));
			t_scaleZ = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));

			pArrDataScale->removeAllObjects();
			pArrDataScale = NULL;

			//--------------------------close file
			pArrData->removeAllObjects();
			pArrData = NULL;

			skinSubMeshCount=t_nSkinSubMesh;

			mesh->setTransform3D(t_RTmat);
			mesh->scale3D(t_scaleX, t_scaleY, t_scaleZ);
		}
		//----m_skinSubMesh_0 ~ m_skinSubMesh_[skinSubMeshCount-1]
		for(int skinSubMeshIndex=0;skinSubMeshIndex<skinSubMeshCount;skinSubMeshIndex++){

			//----FKCWSkinSubMesh.txt
			FKCW_3D_SubMesh*subMesh=new FKCW_3D_SubMesh();
			subMesh->autorelease();
			subMesh->init();
			{
				string fullpath=CCFileUtils::sharedFileUtils()->fullPathForFilename((path+"/m_skinMesh_"+FKCW_Base_Utils::IntToString(skinMeshIndex)+
					"/m_skinSubMesh_"+FKCW_Base_Utils::IntToString(skinSubMeshIndex)+
					"/FKCWSkinSubMesh.txt").c_str());
				
				//open file
				pArrData = getCCArrayByFileName(fullpath.c_str());
				if (pArrData->count() < 5)
				{
					assert(false);
				}
				//------------------read file
				//pArrData[0]
				if(!valueOnly)assert(string(tCharBuffer)=="texFilePath");
				pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(0));
				if (pStr == NULL)
				{
					assert(false);
				}
				string texFilePathRelativeToModelPath = pStr->getCString();
				string texFilePath=path+"/"+texFilePathRelativeToModelPath;
				CCTexture2D*t_texture=CCTextureCache::sharedTextureCache()->addImage(texFilePath.c_str());

				FKCW_3D_Vector4 ambient;
				FKCW_3D_Vector4 diffuse;
				FKCW_3D_Vector4 specular;

				//pArrData[1]
				pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1));
				if (pStr == NULL)
				{
					assert(false);
				}
				if(!valueOnly)assert(string(tCharBuffer)=="ambient");
				ambient = fVector4ByString(pStr->getCString());
				

				//pArrData[2]
				pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(2));
				if (pStr == NULL)
				{
					assert(false);
				}
				if(!valueOnly)assert(string(tCharBuffer)=="diffuse");
				diffuse = fVector4ByString(pStr->getCString());

				//pArrData[3]
				pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(3));
				if (pStr == NULL)
				{
					assert(false);
				}
				if(!valueOnly)assert(string(tCharBuffer)=="specular");
				specular = fVector4ByString(pStr->getCString());

				//pArrData[4]
				pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(4));
				if (pStr == NULL)
				{
					assert(false);
				}
				float shininess;
				if(!valueOnly)assert(string(tCharBuffer)=="shininess");
				shininess = static_cast<float>(FKCW_Base_Utils::AtoF(pStr->getCString()));



				FKCW_3D_Material*t_material=new FKCW_3D_Material();
				t_material->autorelease();
				t_material->setAmbient(ambient);
				t_material->setDiffuse(diffuse);
				t_material->setSpecular(specular);
				t_material->setShininess(shininess);

				//--------------------------close file
				pArrData->removeAllObjects();
				pArrData = NULL;

				subMesh->setTexture(t_texture);
				subMesh->setMaterial(t_material);

			}
			//----m_subMeshData_backup/FKCWSkinSubMeshData.txt
			FKCW_3D_SubMeshData*subMeshData=new FKCW_3D_SubMeshData();
			subMeshData->autorelease();
			subMeshData->init();
			{
				string fullpath=CCFileUtils::sharedFileUtils()->fullPathForFilename((path+"/m_skinMesh_"+FKCW_Base_Utils::IntToString(skinMeshIndex)+
					"/m_skinSubMesh_"+FKCW_Base_Utils::IntToString(skinSubMeshIndex)+
					"/m_subMeshData_backup/FKCWSkinSubMeshData.txt").c_str());
				//open file
				pArrData = getCCArrayByFileName(fullpath.c_str());
				if (pArrData->count() == 0)
				{
					assert(false);
				}

				//------------------read file
				//vlist
				vector<FKCW_3D_Vector4> t_positionList;
				vector<FKCW_3D_Vector2> t_texCoordList;
				vector<FKCW_3D_Vector4> t_normalList;
				vector<FKCW_3D_Vector4> t_colorList;

				//pArrData[0]
				pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(0));
				if (pStr == NULL)
				{
					assert(false);
				}
				int t_nv = FKCW_Base_Utils::AtoI(pStr->getCString());

				if(!valueOnly)assert(string(tCharBuffer)=="m_vlist.size");

				// 3 2 3 3 (4 lines a loop)
				for(int i=0;i<t_nv;i++){
					
					if(!valueOnly)assert(string(tCharBuffer)=="vertex");
					FKCW_3D_Vector4 pos;
					FKCW_3D_Vector2 texCoord;
					FKCW_3D_Vector4 norm;
					FKCW_3D_Vector4 color;
					
					string strTemp;
					//pArrData[1 + 4*i + 0]
					pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1 + 4*i + 0));
					if (pStr == NULL)
					{
						assert(false);
					}
					strTemp = pStr->getCString();
					strTemp += " 1.000000";
					pos = fVector4ByString(strTemp.c_str());

					//pArrData[1 + 4*i + 1]
					pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1 + 4*i + 1));
					if (pStr == NULL)
					{
						assert(false);
					}
					texCoord = fVector2ByString(pStr->getCString());
					
					//pArrData[1 + 4*i + 2]
					pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1 + 4*i + 2));
					if (pStr == NULL)
					{
						assert(false);
					}
					strTemp = pStr->getCString();
					strTemp += " 0.000000";
					norm = fVector4ByString(strTemp.c_str());
					
					//pArrData[1 + 4*i + 3]
					pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1 + 4*i + 3));
					if (pStr == NULL)
					{
						assert(false);
					}
					color = fVector4ByString(pStr->getCString());

					//add to list
					t_positionList.push_back(pos);
					t_texCoordList.push_back(texCoord);
					t_normalList.push_back(norm);
					t_colorList.push_back(color);
				}

				vector<FKCW_3D_IDTriangle> t_IDtriList;
				int t_nIDtri;
				if(!valueOnly)assert(string(tCharBuffer)=="m_IDtriList.size");

				//pArrData[1 + t_nv*4]
				pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1 + t_nv*4));
				if (pStr == NULL)
				{
					assert(false);
				}
				t_nIDtri = FKCW_Base_Utils::AtoI(pStr->getCString());

				for(int i=0;i<t_nIDtri;i++){
					if(!valueOnly)assert(string(tCharBuffer)=="IDtri");
					//pArrData[1 + t_nv*4 + i + 1]
					pStr = dynamic_cast<CCString*>(pArrData->objectAtIndex(1 + t_nv*4 + i + 1));
					if (pStr == NULL)
					{
						assert(false);
					}
					//add to list
					t_IDtriList.push_back(fIDTriangleByString(pStr->getCString()));
				}

				//--------------------------close file
				pArrData->removeAllObjects();
				pArrData = NULL;
				//----
				subMeshData->m_positionList=t_positionList;
				subMeshData->m_normalList=t_normalList;
				subMeshData->m_texCoordList=t_texCoordList;
				subMeshData->m_colorList=t_colorList;
				subMeshData->m_IDtriList=t_IDtriList;
			}
			//----
			subMesh->setSubMeshData(subMeshData);
			mesh->addSubMesh(subMesh);

		}
		//----
		model->addMesh(mesh);
	}
	//----
	actor->setModel(model);
	return actor;
}
//-------------------------------------------------------------------------