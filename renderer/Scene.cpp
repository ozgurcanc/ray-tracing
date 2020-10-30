#include "Scene.h"

void Scene::CreateScene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 0;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if (pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	if (pElement)
	{
		str = pElement->GetText();
		sscanf(str, "%f %f %f", &backgroundColor.x, &backgroundColor.y, &backgroundColor.z);
	}

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if (pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if (pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != nullptr)
	{
		int id;
		char imageName[64];
		Vector3 pos, gaze, up;
		ImagePlane imgPlane;
		float near, fovy;
		bool right_handed = true;
		Camera * cam;

		const char * hand_system = pCamera->Attribute("handedness");
		if (hand_system && strcmp(hand_system, "left") == 0)
			right_handed = false;

		const char* value = pCamera->Attribute("type");

		if (value && strcmp(value, "lookAt") == 0)
		{
			bool gaze_flag = false;

			eResult = pCamera->QueryIntAttribute("id", &id);
			camElement = pCamera->FirstChildElement("Position");
			str = camElement->GetText();
			sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
			camElement = pCamera->FirstChildElement("GazePoint");
			if (camElement)
			{
				str = camElement->GetText();
				sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
			}
			else
			{
				camElement = pCamera->FirstChildElement("Gaze");
				str = camElement->GetText();
				sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
				gaze_flag = true;
			}

			camElement = pCamera->FirstChildElement("Up");
			str = camElement->GetText();
			sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);

			camElement = pCamera->FirstChildElement("FovY");
			camElement->QueryFloatText(&fovy);

			camElement = pCamera->FirstChildElement("NearDistance");
			camElement->QueryFloatText(&near);

			camElement = pCamera->FirstChildElement("ImageResolution");
			str = camElement->GetText();
			sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);

			camElement = pCamera->FirstChildElement("ImageName");
			str = camElement->GetText();
			strcpy(imageName, str);

			cam = new Camera(imageName);
			if (gaze_flag)
				cam->LookAt(pos, pos + gaze, up, right_handed);
			else
				cam->LookAt(pos, gaze, up, right_handed);
			cam->Perspective(fovy * DegToRad, (real)imgPlane.nx / imgPlane.ny, near, imgPlane.nx);
			cameras.push_back(cam);

			camElement = pCamera->FirstChildElement("FocusDistance");
			if (camElement != nullptr)
				camElement->QueryFloatText(&cam->focusDistance);

			camElement = pCamera->FirstChildElement("ApertureSize");
			if (camElement != nullptr)
				camElement->QueryFloatText(&cam->apertureSize);


			camElement = pCamera->FirstChildElement("NumSamples");
			if (camElement != nullptr)
			{
				int numSamples;
				camElement->QueryIntText(&numSamples);
				cam->EnableMSAA(numSamples);
			}

			camElement = pCamera->FirstChildElement("Tonemap");
			if (camElement)
			{
				float key_value;
				float saturation;
				float burn;
				float gamma;
				XMLElement *tmoElement;
				tmoElement = camElement->FirstChildElement("TMOOptions");
				str = tmoElement->GetText();
				sscanf(str, "%f %f", &key_value, &burn);
				tmoElement = camElement->FirstChildElement("Saturation");
				tmoElement->QueryFloatText(&saturation);
				tmoElement = camElement->FirstChildElement("Gamma");
				tmoElement->QueryFloatText(&gamma);

				cam->EnableToneMapping(key_value, burn, saturation, gamma);
			}
		}
		else
		{
			eResult = pCamera->QueryIntAttribute("id", &id);
			camElement = pCamera->FirstChildElement("Position");
			str = camElement->GetText();
			sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
			camElement = pCamera->FirstChildElement("Gaze");
			str = camElement->GetText();
			sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
			camElement = pCamera->FirstChildElement("Up");
			str = camElement->GetText();
			sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
			camElement = pCamera->FirstChildElement("NearPlane");
			str = camElement->GetText();
			sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
			camElement = pCamera->FirstChildElement("NearDistance");
			eResult = camElement->QueryFloatText(&imgPlane.distance);
			camElement = pCamera->FirstChildElement("ImageResolution");
			str = camElement->GetText();
			sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
			camElement = pCamera->FirstChildElement("ImageName");
			str = camElement->GetText();
			strcpy(imageName, str);

			cam = new Camera(imageName);
			cam->LookAt(pos, pos + gaze, up, right_handed);
			cam->Perspective(imgPlane.left, imgPlane.right, imgPlane.bottom, imgPlane.top, imgPlane.distance, imgPlane.nx, imgPlane.ny);
			cameras.push_back(cam);

			camElement = pCamera->FirstChildElement("FocusDistance");
			if (camElement != nullptr)
				camElement->QueryFloatText(&cam->focusDistance);

			camElement = pCamera->FirstChildElement("ApertureSize");
			if (camElement != nullptr)
				camElement->QueryFloatText(&cam->apertureSize);

			camElement = pCamera->FirstChildElement("NumSamples");
			if (camElement != nullptr)
			{
				int numSamples;
				camElement->QueryIntText(&numSamples);
				cam->EnableMSAA(numSamples);
			}

			camElement = pCamera->FirstChildElement("Tonemap");
			if (camElement)
			{
				float key_value;
				float saturation;
				float burn;
				float gamma;
				XMLElement *tmoElement;
				tmoElement = camElement->FirstChildElement("TMOOptions");
				str = tmoElement->GetText();
				sscanf(str, "%f %f", &key_value, &burn);
				tmoElement = camElement->FirstChildElement("Saturation");
				tmoElement->QueryFloatText(&saturation);
				tmoElement = camElement->FirstChildElement("Gamma");
				tmoElement->QueryFloatText(&gamma);

				cam->EnableToneMapping(key_value, burn, saturation, gamma);
			}
		}

		camElement = pCamera->FirstChildElement("Renderer");
		if (camElement)
		{
			str = camElement->GetText();
			if (strcmp(str, "PathTracing") == 0)
			{
				cam->integrator.type = Integrator::IntegratorType::PATH_TRACING;
				camElement = pCamera->FirstChildElement("RendererParams");
				if (camElement)
				{
					char * str1 = (char*)camElement->GetText();
					if (str1)
					{
						char * pch;
						pch = strtok(str1, " ");
						while (pch != NULL)
						{

							if (strcmp("NextEventEstimation", pch) == 0) cam->integrator.NextEventEstimation = true;
							else if (strcmp("ImportanceSampling", pch) == 0) cam->integrator.ImportanceSampling = true;
							else if (strcmp("RussianRoulette", pch) == 0) cam->integrator.RussianRoulette = true;
							pch = strtok(NULL, " ");
						}
					}
				}
			}
		}

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse BRDFs
	std::vector<BRDFs> brdf_types;
	std::vector<float> exponents;
	pElement = pRoot->FirstChildElement("BRDFs");
	XMLElement *pBRDF = nullptr;
	if (pElement) pBRDF = pElement->FirstChildElement();
	while (pBRDF != nullptr)
	{
		const char * name = pBRDF->Name();
		bool normalized = pBRDF->BoolAttribute("normalized");
		bool kdfresnal = pBRDF->BoolAttribute("kdfresnel");
		float exp = 1;
		pBRDF->FirstChildElement("Exponent")->QueryFloatText(&exp);

		if (strcmp(name, "OriginalPhong") == 0)
		{
			brdf_types.push_back(PHONG);
		}
		else if (strcmp(name, "ModifiedPhong") == 0)
		{
			if (normalized) brdf_types.push_back(NORMALIZED_MODIFIED_PHONG);
			else brdf_types.push_back(MODIFIED_PHONG);
		}
		else if (strcmp(name, "OriginalBlinnPhong") == 0)
		{
			brdf_types.push_back(BLINN_PHONG);
		}
		else if (strcmp(name, "ModifiedBlinnPhong") == 0)
		{
			if (normalized) brdf_types.push_back(NORMALIZED_MODIFIED_BLINN_PHONG);
			else brdf_types.push_back(MODIFIED_BLINN_PHONG);
		}
		else if (strcmp(name, "TorranceSparrow") == 0)
		{
			if (kdfresnal) brdf_types.push_back(TORRANCE_SPARROW_DIFFUSE_FRESNEL);
			else brdf_types.push_back(TORRANCE_SPARROW);
		}

		exponents.push_back(exp);

		pBRDF = pBRDF->NextSiblingElement();
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while (pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;

		const char* value = pMaterial->Attribute("type");
		if (value)
		{
			if (strcmp(value, "conductor") == 0) materials[curr]->type = MaterialType::Conductor;
			else if (strcmp(value, "dielectric") == 0) materials[curr]->type = MaterialType::Dielectric;
			else if (strcmp(value, "mirror") == 0) materials[curr]->type = MaterialType::Mirror;
		}


		//eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.x, &materials[curr]->ambientRef.y, &materials[curr]->ambientRef.z);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.x, &materials[curr]->diffuseRef.y, &materials[curr]->diffuseRef.z);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.x, &materials[curr]->specularRef.y, &materials[curr]->specularRef.z);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if (materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.x, &materials[curr]->mirrorRef.y, &materials[curr]->mirrorRef.z);
		}
		else
		{
			materials[curr]->mirrorRef.x = 0.0;
			materials[curr]->mirrorRef.y = 0.0;
			materials[curr]->mirrorRef.z = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if (materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		materialElement = pMaterial->FirstChildElement("Roughness");
		if (materialElement != nullptr)
			materialElement->QueryFloatText(&materials[curr]->roughness);

		materialElement = pMaterial->FirstChildElement("AbsorptionCoefficient");
		if (materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->absorptionCoef.x, &materials[curr]->absorptionCoef.y, &materials[curr]->absorptionCoef.z);
		}

		materialElement = pMaterial->FirstChildElement("RefractionIndex");
		if (materialElement != nullptr)
			materialElement->QueryFloatText(&materials[curr]->refractionIndex);

		materialElement = pMaterial->FirstChildElement("AbsorptionIndex");
		if (materialElement != nullptr)
			materialElement->QueryFloatText(&materials[curr]->absorptionIndex);

		const char* value_degamma = pMaterial->Attribute("degamma");
		if (value_degamma && strcmp(value_degamma, "true") == 0)
		{
			materials[curr]->ambientRef.x = pow(materials[curr]->ambientRef.x, 2.2f);
			materials[curr]->ambientRef.y = pow(materials[curr]->ambientRef.y, 2.2f);
			materials[curr]->ambientRef.z = pow(materials[curr]->ambientRef.z, 2.2f);

			materials[curr]->diffuseRef.x = pow(materials[curr]->diffuseRef.x, 2.2f);
			materials[curr]->diffuseRef.y = pow(materials[curr]->diffuseRef.y, 2.2f);
			materials[curr]->diffuseRef.z = pow(materials[curr]->diffuseRef.z, 2.2f);

			materials[curr]->specularRef.x = pow(materials[curr]->specularRef.x, 2.2f);
			materials[curr]->specularRef.y = pow(materials[curr]->specularRef.y, 2.2f);
			materials[curr]->specularRef.z = pow(materials[curr]->specularRef.z, 2.2f);
		}

		int brdf_index = pMaterial->IntAttribute("BRDF");
		if (brdf_index)
		{
			materials[curr]->brdf = brdf_types[brdf_index - 1];
			materials[curr]->phongExp = exponents[brdf_index - 1];
		}

		pMaterial = pMaterial->NextSiblingElement("Material");

	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	if (pElement)
	{
		int cursor = 0;
		Vector3 tmpPoint;
		str = pElement->GetText();
		while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while (str[cursor] != '\0')
		{
			for (int cnt = 0; cnt < 3; cnt++)
			{
				if (cnt == 0)
					tmpPoint.x = atof(str + cursor);
				else if (cnt == 1)
					tmpPoint.y = atof(str + cursor);
				else
					tmpPoint.z = atof(str + cursor);
				while (str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++;
				while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			Vertex temp;
			temp.position = tmpPoint;
			vertices.push_back(temp);
		}
	}

	//Parse Texture Coordinate
	pElement = pRoot->FirstChildElement("TexCoordData");
	if (pElement)
	{
		int cursor = 0;
		int curr_vertex_index = 0;
		str = pElement->GetText();
		while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while (str[cursor] != '\0')
		{
			for (int cnt = 0; cnt < 2; cnt++)
			{
				if (cnt == 0)
					vertices[curr_vertex_index].texCoord.x = atof(str + cursor);
				else
					vertices[curr_vertex_index].texCoord.y = atof(str + cursor);
				while (str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++;
				while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			curr_vertex_index++;
		}
	}

	// Parse Textures
	std::vector<Image *> images;
	pElement = pRoot->FirstChildElement("Textures");
	if (pElement)
	{

		XMLElement * image = pElement->FirstChildElement("Images");
		if (image)
		{
			const char *imagePath;
			image = image->FirstChildElement("Image");
			while (image)
			{
				imagePath = image->GetText();
				images.push_back(new Image(imagePath));
				image = image->NextSiblingElement("Image");
			}
		}

		pElement = pElement->FirstChildElement("TextureMap");
		XMLElement * texElement;
		while (pElement)
		{
			TextureUsage usage;
			real bumpFactor = 1;

			texElement = pElement->FirstChildElement("DecalMode");
			str = texElement->GetText();
			if (strcmp(str, "replace_all") == 0) usage = REPLACE_ALL;
			else if (strcmp(str, "replace_kd") == 0) usage = REPLACE_KD;
			else if (strcmp(str, "blend_kd") == 0) usage = BLEND_KD;
			else if (strcmp(str, "replace_normal") == 0) usage = REPLACE_NORMAL;
			else if (strcmp(str, "bump_normal") == 0) usage = BUMP_NORMAL;
			else usage = REPLACE_BACKGROUND;

			texElement = pElement->FirstChildElement("BumpFactor");
			if (texElement)
			{
				texElement->QueryFloatText(&bumpFactor);
				bumpFactor = bumpFactor > 1 ? 2 : bumpFactor;
			}

			const char* value = pElement->Attribute("type");
			if (strcmp(value, "image") == 0)
			{
				Interpolation interpolation = BILINEAR;

				texElement = pElement->FirstChildElement("Interpolation");
				if (texElement)
				{
					str = texElement->GetText();
					if (strcmp(str, "nearest") == 0) interpolation = NEAREST_NEIGHBOR;
					else if (strcmp(str, "bilinear") == 0) interpolation = BILINEAR;
				}

				texElement = pElement->FirstChildElement("ImageId");
				int id;
				texElement->QueryIntText(&id);

				textures.push_back(new ImageTexture(images[id - 1], interpolation, usage, bumpFactor));

				if (usage == REPLACE_BACKGROUND)
				{
					backgroundTexture = textures.back();
				}
			}
			else if (strcmp(value, "perlin") == 0)
			{
				NoiseConversion noise;
				real noiseScale = 1;

				texElement = pElement->FirstChildElement("NoiseConversion");
				str = texElement->GetText();
				if (strcmp(str, "absval") == 0) noise = ABSOLUTE;
				else if (strcmp(str, "linear") == 0) noise = LINEAR;

				texElement = pElement->FirstChildElement("NoiseScale");
				if (texElement) texElement->QueryFloatText(&noiseScale);

				textures.push_back(new PerlinTexture(usage, noise, noiseScale, bumpFactor));
			}

			pElement = pElement->NextSiblingElement("TextureMap");
		}
	}

	//Parse Transformation
	std::vector<Matrix4> translation;
	std::vector<Matrix4> scaling;
	std::vector<Matrix4> rotation;
	std::vector<Matrix4> composite;

	pElement = pRoot->FirstChildElement("Transformations");
	if (pElement != nullptr)
	{
		XMLElement *transformation = pElement->FirstChildElement("Translation");
		int transformationID;
		real x, y, z, angle;

		while (transformation != nullptr)
		{
			transformation->QueryIntAttribute("id", &transformationID);
			str = transformation->GetText();
			sscanf(str, "%f %f %f", &x, &y, &z);
			translation.push_back(Matrix4::Translate(x, y, z));

			transformation = transformation->NextSiblingElement("Translation");
		}

		transformation = pElement->FirstChildElement("Scaling");
		while (transformation != nullptr)
		{
			transformation->QueryIntAttribute("id", &transformationID);
			str = transformation->GetText();
			sscanf(str, "%f %f %f", &x, &y, &z);
			scaling.push_back(Matrix4::Scale(x, y, z));

			transformation = transformation->NextSiblingElement("Scaling");
		}

		transformation = pElement->FirstChildElement("Rotation");
		while (transformation != nullptr)
		{
			transformation->QueryIntAttribute("id", &transformationID);
			str = transformation->GetText();
			sscanf(str, "%f %f %f %f", &angle, &x, &y, &z);
			rotation.push_back(Matrix4::Rotate(angle, x, y, z));

			transformation = transformation->NextSiblingElement("Rotation");
		}

		transformation = pElement->FirstChildElement("Composite");
		while (transformation != nullptr)
		{
			Matrix4 m;
			transformation->QueryIntAttribute("id", &transformationID);
			str = transformation->GetText();
			sscanf(str, "%f %f %f %f %f %f %f %f %f %f %f %f", &m.data[0], &m.data[1], &m.data[2], &m.data[3],
				&m.data[4], &m.data[5], &m.data[6], &m.data[7],
				&m.data[8], &m.data[9], &m.data[10], &m.data[11]);
			composite.push_back(m);

			transformation = transformation->NextSiblingElement("Composite");
		}
	}


	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");

	Shape * sphere = new Sphere();
	primitives.push_back(PrimitiveInfo(sphere, Vector3(), 1));

	BVH *sphere_local_bvh = new BVH(primitives, 0, 1);

	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while (pObject != nullptr)
	{
		int start_index = primitives.size();

		int id;
		int matIndex;
		int cIndex;
		float R;

		Vector3 motion;
		Matrix4 transform;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		int texIndex[2] = { -1,-1 };
		objElement = pObject->FirstChildElement("Textures");
		if (objElement)
		{
			char * tex = (char *)objElement->GetText();
			char * pch = strtok(tex, " ");
			texIndex[0] = std::stoi(&pch[0]) - 1;
			pch = strtok(NULL, " ");
			if (pch) texIndex[1] = std::stoi(&pch[0]) - 1;
		}

		objElement = pObject->FirstChildElement("MotionBlur");
		if (objElement != nullptr)
		{
			str = objElement->GetText();
			sscanf(str, "%f %f %f", &motion.x, &motion.y, &motion.z);
		}

		Vector3 center = vertices[cIndex - 1].position;
		transform = Matrix4::Translate(center.x, center.y, center.z) * Matrix4::Scale(R, R, R);

		objElement = pObject->FirstChildElement("Transformations");
		if (objElement != nullptr)
		{
			char * str1;
			str1 = (char*)objElement->GetText();

			char * pch;
			char type;
			int index;
			pch = strtok(str1, " ");

			while (pch != NULL)
			{
				type = pch[0];
				index = std::stoi(&pch[1]);

				if (type == 't') transform = translation[index - 1] * transform;
				else if (type == 's') transform = scaling[index - 1] * transform;
				else if (type == 'r') transform = rotation[index - 1] * transform;
				else if (type == 'c') transform = composite[index - 1] * transform;

				pch = strtok(NULL, " ");
			}
		}

		objects.push_back(new ObjectInfo(sphere_local_bvh, transform, matIndex - 1, motion, false, texIndex));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while (pObject != nullptr)
	{
		int start_index = primitives.size();

		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);

		bool smoothShading = false;
		const char* value = pObject->Attribute("shadingMode");
		if (value && strcmp(value, "smooth") == 0)
		{
			smoothShading = true;
		}

		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		Shape *triangle = new Triangle(p1Index - 1, p2Index - 1, p3Index - 1);
		primitives.push_back(PrimitiveInfo(triangle, vertices[p1Index - 1].position, vertices[p2Index - 1].position, vertices[p3Index - 1].position));

		BVH *temp = new BVH(primitives, start_index, start_index + 1);

		if (smoothShading) ComputeNormal(vertices, p1Index - 1, p2Index - 1, p3Index - 1);

		Vector3 motion;
		objElement = pObject->FirstChildElement("MotionBlur");
		if (objElement != nullptr)
		{
			str = objElement->GetText();
			sscanf(str, "%f %f %f", &motion.x, &motion.y, &motion.z);
		}

		int texIndex[2] = { -1,-1 };
		objElement = pObject->FirstChildElement("Textures");
		if (objElement)
		{
			char * tex = (char *)objElement->GetText();
			char * pch = strtok(tex, " ");
			texIndex[0] = std::stoi(&pch[0]) - 1;
			pch = strtok(NULL, " ");
			if (pch) texIndex[1] = std::stoi(&pch[0]) - 1;
		}

		Matrix4 transform;
		objElement = pObject->FirstChildElement("Transformations");
		if (objElement != nullptr)
		{
			char * str1;
			str1 = (char*)objElement->GetText();

			char * pch;
			char type;
			int index;
			pch = strtok(str1, " ");

			while (pch != NULL)
			{
				type = pch[0];
				index = std::stoi(&pch[1]);

				if (type == 't') transform = translation[index - 1] * transform;
				else if (type == 's') transform = scaling[index - 1] * transform;
				else if (type == 'r') transform = rotation[index - 1] * transform;
				else if (type == 'c') transform = composite[index - 1] * transform;

				pch = strtok(NULL, " ");
			}
		}

		objects.push_back(new ObjectInfo(temp, transform, matIndex - 1, motion, smoothShading, texIndex));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	int meshStartIndex = objects.size();

	pObject = pElement->FirstChildElement("Mesh");
	while (pObject != nullptr)
	{
		int start_index = primitives.size();

		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		int textureOffset = 0;
		std::vector<Triangle> faces;
		std::vector<int> *meshIndices = new std::vector<int>();

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");

		bool smoothShading = false;
		const char * shading = pObject->Attribute("shadingMode");
		if (shading && strcmp(shading, "smooth") == 0)
		{
			smoothShading = true;
		}

		objElement->QueryIntAttribute("textureOffset", &textureOffset);

		const char* value = objElement->Attribute("plyFile");

		if (value)
		{
			ParsePLYFile(value, matIndex - 1, smoothShading);
		}
		else
		{
			objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
			str = objElement->GetText();
			while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
			while (str[cursor] != '\0')
			{
				for (int cnt = 0; cnt < 3; cnt++)
				{
					if (cnt == 0)
						p1Index = atoi(str + cursor) + vertexOffset;
					else if (cnt == 1)
						p2Index = atoi(str + cursor) + vertexOffset;
					else
						p3Index = atoi(str + cursor) + vertexOffset;
					while (str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
						cursor++;
					while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
						cursor++;
				}
				faces.push_back(*(new Triangle(p1Index - 1, p2Index - 1, p3Index - 1)));
				meshIndices->push_back(p1Index);
				meshIndices->push_back(p2Index);
				meshIndices->push_back(p3Index);

				if (smoothShading) ComputeNormal(vertices, p1Index - 1, p2Index - 1, p3Index - 1);

				Shape *triangle = new Triangle(p1Index - 1, p2Index - 1, p3Index - 1);
				primitives.push_back(PrimitiveInfo(triangle, vertices[p1Index - 1].position, vertices[p2Index - 1].position, vertices[p3Index - 1].position));

			}
		}

		Matrix4 transform;
		Vector3 motion;

		int texIndex[2] = { -1,-1 };
		objElement = pObject->FirstChildElement("Textures");
		if (objElement)
		{
			char * tex = (char *)objElement->GetText();
			char * pch = strtok(tex, " ");
			texIndex[0] = std::stoi(&pch[0]) - 1;
			pch = strtok(NULL, " ");
			if (pch) texIndex[1] = std::stoi(&pch[0]) - 1;
		}

		objElement = pObject->FirstChildElement("Transformations");
		if (objElement != nullptr)
		{
			char * str1;
			str1 = (char*)objElement->GetText();

			char * pch;
			char type;
			int index;
			pch = strtok(str1, " ");

			while (pch != NULL)
			{
				type = pch[0];
				index = std::stoi(&pch[1]);

				if (type == 't') transform = translation[index - 1] * transform;
				else if (type == 's') transform = scaling[index - 1] * transform;
				else if (type == 'r') transform = rotation[index - 1] * transform;
				else if (type == 'c') transform = composite[index - 1] * transform;

				pch = strtok(NULL, " ");
			}
		}

		objElement = pObject->FirstChildElement("MotionBlur");
		if (objElement != nullptr)
		{
			str = objElement->GetText();
			sscanf(str, "%f %f %f", &motion.x, &motion.y, &motion.z);
		}

		int end_index = primitives.size();

		BVH * temp = new BVH(primitives, start_index, end_index);

		objects.push_back(new ObjectInfo(temp, transform, matIndex - 1, motion, smoothShading, texIndex, textureOffset - vertexOffset));

		pObject = pObject->NextSiblingElement("Mesh");

	}

	// Parse mesh-light
	pObject = pElement->FirstChildElement("LightMesh");
	while (pObject != nullptr)
	{
		int start_index = primitives.size();

		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		int textureOffset = 0;
		std::vector<Triangle> faces;
		std::vector<int> *meshIndices = new std::vector<int>();

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");

		bool smoothShading = false;
		const char * shading = pObject->Attribute("shadingMode");
		if (shading && strcmp(shading, "smooth") == 0)
		{
			smoothShading = true;
		}

		objElement->QueryIntAttribute("textureOffset", &textureOffset);

		const char* value = objElement->Attribute("plyFile");
		if (value)
		{
			ParsePLYFile(value, matIndex - 1, smoothShading);
		}
		else
		{
			objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
			str = objElement->GetText();
			while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
			while (str[cursor] != '\0')
			{
				for (int cnt = 0; cnt < 3; cnt++)
				{
					if (cnt == 0)
						p1Index = atoi(str + cursor) + vertexOffset;
					else if (cnt == 1)
						p2Index = atoi(str + cursor) + vertexOffset;
					else
						p3Index = atoi(str + cursor) + vertexOffset;
					while (str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
						cursor++;
					while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
						cursor++;
				}
				faces.push_back(*(new Triangle(p1Index - 1, p2Index - 1, p3Index - 1)));
				meshIndices->push_back(p1Index);
				meshIndices->push_back(p2Index);
				meshIndices->push_back(p3Index);

				if (smoothShading) ComputeNormal(vertices, p1Index - 1, p2Index - 1, p3Index - 1);

				Shape *triangle = new Triangle(p1Index - 1, p2Index - 1, p3Index - 1);
				primitives.push_back(PrimitiveInfo(triangle, vertices[p1Index - 1].position, vertices[p2Index - 1].position, vertices[p3Index - 1].position));

			}
		}
		Vector3 radiance;

		objElement = pObject->FirstChildElement("Radiance");
		if (objElement)
		{
			str = objElement->GetText();
			sscanf(str, "%f %f %f", &radiance.x, &radiance.y, &radiance.z);
		}

		Matrix4 transform;
		Vector3 motion;
		int texIndex[2] = { -1,-1 };

		int end_index = primitives.size();

		BVH * temp = new BVH(primitives, start_index, end_index);

		std::vector<real> cdf;
		real total_area = 0;
		for (int i = start_index; i < end_index; i++)
		{
			total_area += primitives[i].primitive->Area(vertices);
			cdf.push_back(total_area);
		}

		for (int i = 0; i < cdf.size(); i++)
		{
			cdf[i] /= total_area;
		}

		LightMesh * lm = new LightMesh(temp, transform, matIndex - 1, motion, smoothShading, texIndex, textureOffset - vertexOffset);
		lm->radiance = radiance;
		lm->weight_cdf = cdf;
		lm->primitive_start = start_index;

		lights.push_back((Light*)lm);
		objects.push_back((ObjectInfo*)lm);

		pObject = pObject->NextSiblingElement("LightMesh");

	}

	// Parse light-sphere
	pObject = pElement->FirstChildElement("LightSphere");
	while (pObject != nullptr)
	{
		int start_index = primitives.size();

		int id;
		int matIndex;
		int cIndex;
		float R;

		Vector3 motion;
		Matrix4 transform;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		int texIndex[2] = { -1,-1 };
		objElement = pObject->FirstChildElement("Textures");
		if (objElement)
		{
			char * tex = (char *)objElement->GetText();
			char * pch = strtok(tex, " ");
			texIndex[0] = std::stoi(&pch[0]) - 1;
			pch = strtok(NULL, " ");
			if (pch) texIndex[1] = std::stoi(&pch[0]) - 1;
		}

		objElement = pObject->FirstChildElement("MotionBlur");
		if (objElement != nullptr)
		{
			str = objElement->GetText();
			sscanf(str, "%f %f %f", &motion.x, &motion.y, &motion.z);
		}

		Vector3 center = vertices[cIndex - 1].position;
		transform = Matrix4::Translate(center.x, center.y, center.z) * Matrix4::Scale(R, R, R);

		objElement = pObject->FirstChildElement("Transformations");
		if (objElement != nullptr)
		{
			char * str1;
			str1 = (char*)objElement->GetText();

			char * pch;
			char type;
			int index;
			pch = strtok(str1, " ");

			while (pch != NULL)
			{
				type = pch[0];
				index = std::stoi(&pch[1]);

				if (type == 't') transform = translation[index - 1] * transform;
				else if (type == 's') transform = scaling[index - 1] * transform;
				else if (type == 'r') transform = rotation[index - 1] * transform;
				else if (type == 'c') transform = composite[index - 1] * transform;


				pch = strtok(NULL, " ");
			}
		}

		Vector3 radiance;
		objElement = pObject->FirstChildElement("Radiance");
		if (objElement)
		{
			str = objElement->GetText();
			sscanf(str, "%f %f %f", &radiance.x, &radiance.y, &radiance.z);
		}

		LightSphere * lp = new LightSphere(sphere_local_bvh, transform, matIndex - 1, motion, false, texIndex, 0);
		lp->radiance = radiance;
		objects.push_back((ObjectInfo*)lp);
		lights.push_back((Light*)lp);

		pObject = pObject->NextSiblingElement("LightSphere");
	}

	// Parse meshe Instances
	pObject = pElement->FirstChildElement("MeshInstance");
	while (pObject != nullptr)
	{
		int matIndex;
		int baseMesh;
		bool resetTransform = false;
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);

		eResult = pObject->QueryIntAttribute("baseMeshId", &baseMesh);
		pObject->QueryBoolAttribute("resetTransform", &resetTransform);

		bool smoothShading = false;
		const char * shading = pObject->Attribute("shadingMode");
		if (shading && strcmp(shading, "smooth") == 0)
		{
			smoothShading = true;
		}

		Matrix4 transform;
		Vector3 motion;

		BVH *temp;
		temp = objects[meshStartIndex + baseMesh - 1]->object;

		objElement = pObject->FirstChildElement("MotionBlur");
		if (objElement != nullptr)
		{
			str = objElement->GetText();
			sscanf(str, "%f %f %f", &motion.x, &motion.y, &motion.z);
		}

		objElement = pObject->FirstChildElement("Transformations");
		if (objElement != nullptr)
		{
			char * str1;
			str1 = (char*)objElement->GetText();

			char * pch;
			char type;
			int index;
			pch = strtok(str1, " ");

			while (pch != NULL)
			{
				type = pch[0];
				index = std::stoi(&pch[1]);

				if (type == 't') transform = translation[index - 1] * transform;
				else if (type == 's') transform = scaling[index - 1] * transform;
				else if (type == 'r') transform = rotation[index - 1] * transform;
				else if (type == 'c') transform = composite[index - 1] * transform;


				pch = strtok(NULL, " ");
			}

			if (!resetTransform)
			{
				transform = transform * objects[meshStartIndex + baseMesh - 1]->inverseTransformation.GetInverse();
			}
		}

		int texIndex[2] = { -1,-1 };
		objElement = pObject->FirstChildElement("Textures");
		if (objElement)
		{
			char * tex = (char *)objElement->GetText();
			char * pch = strtok(tex, " ");
			texIndex[0] = std::stoi(&pch[0]) - 1;
			pch = strtok(NULL, " ");
			if (pch) texIndex[1] = std::stoi(&pch[0]) - 1;
		}

		objects.push_back(new ObjectInfo(temp, transform, matIndex - 1, motion, smoothShading, texIndex));

		pObject = pObject->NextSiblingElement("MeshInstance");
	}

	// Parse lights
	int id;
	Vector3 position;
	Vector3 intensity;
	pElement = pRoot->FirstChildElement("Lights");
	if (pElement)
	{
		XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
		if (pLight)
		{
			str = pLight->GetText();
			sscanf(str, "%f %f %f", &ambientLight.x, &ambientLight.y, &ambientLight.z);
		}
		else
		{
			ambientLight = Vector3(0, 0, 0);
		}

		XMLElement *lightElement;
		pLight = pElement->FirstChildElement("PointLight");
		while (pLight != nullptr)
		{
			eResult = pLight->QueryIntAttribute("id", &id);
			lightElement = pLight->FirstChildElement("Position");
			str = lightElement->GetText();
			sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
			lightElement = pLight->FirstChildElement("Intensity");
			str = lightElement->GetText();
			sscanf(str, "%f %f %f", &intensity.x, &intensity.y, &intensity.z);

			lights.push_back(new PointLight(position, intensity));

			pLight = pLight->NextSiblingElement("PointLight");
		}

		Vector3 direction;
		Vector3 radiance;
		pLight = pElement->FirstChildElement("DirectionalLight");
		while (pLight != nullptr)
		{
			eResult = pLight->QueryIntAttribute("id", &id);
			lightElement = pLight->FirstChildElement("Direction");
			str = lightElement->GetText();
			sscanf(str, "%f %f %f", &direction.x, &direction.y, &direction.z);
			lightElement = pLight->FirstChildElement("Radiance");
			str = lightElement->GetText();
			sscanf(str, "%f %f %f", &radiance.x, &radiance.y, &radiance.z);

			lights.push_back(new DirectionalLight(direction, radiance));

			pLight = pLight->NextSiblingElement("DirectionalLight");
		}

		real cosa;
		real cosb;
		pLight = pElement->FirstChildElement("SpotLight");
		while (pLight != nullptr)
		{
			real spot = 4;
			eResult = pLight->QueryIntAttribute("id", &id);
			lightElement = pLight->FirstChildElement("Position");
			str = lightElement->GetText();
			sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
			lightElement = pLight->FirstChildElement("Direction");
			str = lightElement->GetText();
			sscanf(str, "%f %f %f", &direction.x, &direction.y, &direction.z);
			lightElement = pLight->FirstChildElement("Intensity");
			str = lightElement->GetText();
			sscanf(str, "%f %f %f", &intensity.x, &intensity.y, &intensity.z);

			lightElement = pLight->FirstChildElement("CoverageAngle");
			lightElement->QueryFloatText(&cosb);
			lightElement = pLight->FirstChildElement("FalloffAngle");
			lightElement->QueryFloatText(&cosa);

			lightElement = pLight->FirstChildElement("Spot");
			if (lightElement) lightElement->QueryFloatText(&spot);

			lights.push_back(new SpotLight(position, direction, intensity, cosa, cosb, spot));

			pLight = pLight->NextSiblingElement("SpotLight");
		}

		Vector3 normal;
		real size;
		pLight = pElement->FirstChildElement("AreaLight");
		while (pLight != nullptr)
		{
			eResult = pLight->QueryIntAttribute("id", &id);
			lightElement = pLight->FirstChildElement("Position");
			str = lightElement->GetText();
			sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
			lightElement = pLight->FirstChildElement("Radiance");
			str = lightElement->GetText();
			sscanf(str, "%f %f %f", &radiance.x, &radiance.y, &radiance.z);
			lightElement = pLight->FirstChildElement("Normal");
			str = lightElement->GetText();
			sscanf(str, "%f %f %f", &normal.x, &normal.y, &normal.z);

			lightElement = pLight->FirstChildElement("Size");
			lightElement->QueryFloatText(&size);

			lights.push_back(new AreaLight(position, normal, radiance, size));

			pLight = pLight->NextSiblingElement("AreaLight");
		}

		pLight = pElement->FirstChildElement("SphericalDirectionalLight");
		if (pLight)
		{
			int imageId;
			lightElement = pLight->FirstChildElement("ImageId");
			lightElement->QueryIntText(&imageId);
			lights.push_back(new EnvironmentLight(images[imageId - 1]));
			enviromentLight = (EnvironmentLight*)lights.back();
		}
	}


	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->object_id = i;
	}

	for (Image * i : images)
	{
		delete i;
	}

	for (Vertex vertex : vertices)
	{
		vertex.normal.Normalise();
	}

	pElement = pRoot->FirstChildElement("Integrator");
	if (pElement)
	{
		str = pElement->GetText();
		if (strcmp(str, "PhotonMapping") == 0)
			Build_Photon_Map();
	}
}

void Scene::RenderScene()
{

	int numCore = std::thread::hardware_concurrency();

	for (Camera * cam : cameras)
	{
		integrator = cam->integrator;

		cam->DeriveInternals();
		Vector3 resolution = cam->GetResolution();
		int nx = resolution.x;
		int ny = resolution.y;

		Image* renderedImage = new Image(nx, ny);

		if (numCore == 0)
		{
			RenderHelper(0, 1, renderedImage, nx, ny, cam);
		}
		else
		{
			std::thread* threads = new std::thread[numCore];

			for (int i = 0; i < numCore; i++)
			{
				threads[i] = std::thread(&Scene::RenderHelper, this, i, numCore, renderedImage, nx, ny, cam);
			}

			for (int i = 0; i < numCore; i++) threads[i].join();
		}

		renderedImage->SaveImage(cam->imageName, cam->tmo);

		delete renderedImage;
	}
}

Vector3 Scene::CalculatePixelColor(Ray ray, int currentRecursionDepth)
{

	if (currentRecursionDepth <= 0)
	{
		return Vector3();
	}

	RayHit * rayHit = new RayHit();

	for (ObjectInfo * object : objects)
	{
		object->Intersect(ray, primitives, vertices, rayHit);
	}


	if (rayHit->t == REAL_MAX)
	{
		if (enviromentLight)
		{
			delete rayHit;
			return enviromentLight->EnviromentColor(ray.direction);
		}
		
		if (backgroundTexture)
		{
			rayHit->texCoord = ray.pixelCoord;
			Vector3 background_color = backgroundTexture->Sample(rayHit);
			delete rayHit;
			return background_color;
		}
		else
		{
			delete rayHit;
			return backgroundColor;
		}
	}

	Material material = *(materials.at(rayHit->matIndex));

	Vector3 pixelColor;

	bool shading = rayHit->normal * rayHit->toEye > 0;

	for (int texIndex : rayHit->texIndex)
	{
		if (texIndex == -1) continue;

		Vector3 texValue = textures[texIndex]->Sample(rayHit);

		switch (textures[texIndex]->textureUsage)
		{
		case REPLACE_ALL:
			pixelColor = texValue;
			shading = false;
			break;

		case REPLACE_KD:
			material.diffuseRef = texValue;
			break;

		case BLEND_KD:
			material.diffuseRef = (material.diffuseRef + texValue) * 0.5;
			break;

		case REPLACE_NORMAL:
		case BUMP_NORMAL:
			rayHit->normal = texValue;
			rayHit->normal.Normalise();
			break;

		default:
			break;
		}
	}

	pixelColor += objects[rayHit->object_id]->radiance;

	if (shading)
	{
		pixelColor += ambientLight.ComponentProduct(material.ambientRef);

		if (integrator.type == Integrator::IntegratorType::PATH_TRACING)
		{
			real max_reflectivity = std::max(material.diffuseRef.x + material.specularRef.x, material.diffuseRef.y + material.specularRef.y);
			max_reflectivity = std::max(max_reflectivity, material.diffuseRef.z + material.specularRef.z);

			if (max_reflectivity)
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<> rng(0, 1);

				if (!(integrator.RussianRoulette && rng(gen) > max_reflectivity))
				{
					Vector3 path_dir;
					Vector3 u, v;
					rayHit->normal.OrthonormalBasis(u, v);

					real mult;

					if (integrator.ImportanceSampling)
					{
						real s2 = rng(gen);
						real sqr_s2 = real_sqrt(s2);
						real sqr_one_minus_s2 = real_sqrt(1 - s2);
						real s1 = rng(gen) * 2 * R_PI;
						path_dir = u * sqr_s2 * real_cos(s1) + rayHit->normal * sqr_one_minus_s2 + v * sqr_s2 * real_sin(s1);
						path_dir.Normalise();
						mult = 2 * R_PI * (rayHit->normal * path_dir);
					}
					else
					{
						real s2 = rng(gen);
						real sqr_s2 = real_sqrt(1 - s2 * s2);
						real s1 = rng(gen) * 2 * R_PI;
						path_dir = u * sqr_s2 * real_cos(s1) + rayHit->normal * s2 + v * sqr_s2 * real_sin(s1);
						path_dir.Normalise();
						mult = 2 * R_PI;
					}

					Vector3 path = material.BRDF(rayHit->toEye, rayHit->normal, path_dir);
					Ray path_ray(rayHit->pos + rayHit->normal * 0.001, path_dir, ray.time);
					path *= CalculatePixelColor(path_ray, currentRecursionDepth - 1) * mult;
					pixelColor += path;
				}
			}

			if (!integrator.NextEventEstimation) goto recursive_ray_tracing;
		}

		if (global_photon_map)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> rng(0, 1);

			Vector3 u, v;
			rayHit->normal.OrthonormalBasis(u, v);

			Vector3 direction;
			real s2 = rng(gen);
			real sqr_s2 = real_sqrt(s2);
			real sqr_one_minus_s2 = real_sqrt(1 - s2);
			real s1 = rng(gen) * 2 * R_PI;

			direction = u * sqr_s2 * real_cos(s1) + rayHit->normal * sqr_one_minus_s2 + v * sqr_s2 * real_sin(s1);
			direction.Normalise();

			Ray diffusely_reflected_ray(rayHit->pos + rayHit->normal * 0.001, direction, ray.time);
			pixelColor += Radiance(diffusely_reflected_ray, 5);
		}
		if (caustic_photon_map)
		{
			pixelColor += caustic_photon_map->Radiance_Estimate(rayHit->pos, rayHit->normal, 0.04, 100);
		}


		for (Light *light : lights)
		{
			pixelColor += light->ComputeLightContribution(material, rayHit, ray.time, objects, primitives, vertices);
		}

	}

recursive_ray_tracing:

	if (material.type == MaterialType::Dielectric)
	{
		real a = material.refractionIndex;
		pixelColor += Dielectric(rayHit->toEye, ray.origin, rayHit, (real)1, a, material.absorptionCoef, currentRecursionDepth, ray.time);
	}

	else if (material.type == MaterialType::Conductor)
	{
		pixelColor += Conductor(rayHit, material, currentRecursionDepth, ray.time);
	}

	else if (material.mirrorRef != Vector3())
	{
		Vector3 refDirection = ray.direction - rayHit->normal * 2 * (ray.direction * rayHit->normal);
		refDirection.Normalise();

		Ray reflectionRay;
		if (material.roughness) reflectionRay = RoughReflectionRay(rayHit->pos + rayHit->normal * shadowRayEps, refDirection, material.roughness, ray.time);
		else reflectionRay = Ray(rayHit->pos + rayHit->normal * shadowRayEps, refDirection, ray.time);

		pixelColor += CalculatePixelColor(reflectionRay, currentRecursionDepth - 1).ComponentProduct(material.mirrorRef);
	}

	delete rayHit;
	return pixelColor;
}

void Scene::RenderHelper(int min, int numCore, Image * image, int nx, int ny, Camera *cam)
{

	int i, j;

	while (cam->GetPixel(i, j))
	{
		if (cam->MSAA)
		{
			std::vector<Sample> samples;
			cam->GetSamples(samples, i, j);

			int nSamples = samples.size();

			for (int i = 0; i < nSamples; i++)
			{
				samples[i].color = CalculatePixelColor(samples[i].ray, maxRecursionDepth + 1);
			}

			Vector3 pixelColor = cam->FilteredColor(samples);
			image->SetPixelValue(i, j, pixelColor);
		}
		else
		{
			Ray ray = cam->GetPrimaryRay(i, j);
			Vector3 pixelColor = CalculatePixelColor(ray, maxRecursionDepth + 1);
			image->SetPixelValue(i, j, pixelColor);
		}
	}
}


Vector3 Scene::Dielectric(const Vector3& wo, const Vector3& rayOrigin, const RayHit* rayhit, real etaI, real etaT, const Vector3& absorption, int currRecursionDepth, real time)
{
	real fresnel;
	Vector3 wt, wr;
	Vector3 n = rayhit->normal;

	real cosI = wo * n;

	bool leaving = cosI < real(0);
	if (leaving)
	{
		std::swap(etaI, etaT);
		n = -n;
		cosI = real_abs(cosI);
	}

	real eta = etaI / etaT;

	real sin2I = std::max(real(0), 1 - cosI * cosI);
	real sin2T = eta * eta * sin2I;

	if (sin2T >= 1)
	{
		fresnel = 1;
	}
	else
	{
		real cosT = real_sqrt(1 - sin2T);

		real parallel = (etaT * cosI - etaI * cosT) / (etaT * cosI + etaI * cosT);
		real perpendicular = (etaI * cosI - etaT * cosT) / (etaI * cosI + etaT * cosT);

		fresnel = (parallel * parallel + perpendicular * perpendicular) / 2;
		wt = wo * -eta + n * (eta * cosI - cosT);
		wt.Normalise();
	}

	wr = -wo + n * cosI * 2;
	wr.Normalise();

	Vector3 origin = rayhit->pos + n * shadowRayEps;

	Ray reflection(origin, wr, time);

	Vector3 color = CalculatePixelColor(reflection, currRecursionDepth - 1) * fresnel;

	if (fresnel != (real)1)
	{
		Ray refraction(rayhit->pos - n * shadowRayEps, wt, time);
		color += CalculatePixelColor(refraction, currRecursionDepth - 1) * (1 - fresnel);
	}


	if (leaving)
	{
		real dist = -(rayhit->pos - rayOrigin).Magnitude();
		Vector3 absorb = Vector3(real_exp(absorption.x * dist), real_exp(absorption.y * dist), real_exp(absorption.z * dist));
		color = color.ComponentProduct(absorb);
	}


	return color;
}


Vector3 Scene::Conductor(const RayHit * rayhit, const Material &materail, int curr_recusion_depth, real time)
{
	real cosI = rayhit->toEye * rayhit->normal;
	real cos2I = cosI * cosI;

	const real eta = materail.refractionIndex;
	const real eta_k = materail.absorptionIndex;

	real two_cos_eta = 2 * eta * cosI;

	real t0 = eta * eta + eta_k * eta_k;
	real t1 = t0 * cos2I;

	real rs = (t0 - two_cos_eta + cos2I) / (t0 + two_cos_eta + cos2I);
	real rp = (t1 - two_cos_eta + 1) / (t1 + two_cos_eta + 1);

	real fresnel = (rs + rp) / 2;

	Vector3 wr = -rayhit->toEye + rayhit->normal * cosI * 2;
	wr.Normalise();

	Ray reflection;
	if (materail.roughness) reflection = RoughReflectionRay(rayhit->pos + rayhit->normal * shadowRayEps, wr, materail.roughness, time);
	else reflection = Ray(rayhit->pos + rayhit->normal * shadowRayEps, wr, time);

	return CalculatePixelColor(reflection, curr_recusion_depth - 1).ComponentProduct(materail.mirrorRef) * fresnel;
}


void Scene::ParsePLYFile(const char* filename, int matIndex, bool smooth)
{
	int offset = vertices.size();

	happly::PLYData plyIn(filename);

	bool hasTexCoord = plyIn.getElement("vertex").hasProperty("u");

	std::vector<double> xPos = plyIn.getElement("vertex").getProperty<double>("x");
	std::vector<double> yPos = plyIn.getElement("vertex").getProperty<double>("y");
	std::vector<double> zPos = plyIn.getElement("vertex").getProperty<double>("z");
	std::vector<double> u;
	std::vector<double> v;

	if (hasTexCoord)
	{
		u = plyIn.getElement("vertex").getProperty<double>("u");
		v = plyIn.getElement("vertex").getProperty<double>("v");
	}

	for (size_t i = 0; i < xPos.size(); i++)
	{
		Vertex vertex;
		vertex.position = Vector3(xPos[i], yPos[i], zPos[i]);
		if (hasTexCoord) vertex.texCoord = Vector2(u[i], v[i]);
		vertices.push_back(vertex);
	}

	std::vector<std::vector<int>> fInd = plyIn.getFaceIndices<int>();

	if (fInd[0].size() == 3)
	{
		for (std::vector<int> face : fInd)
		{
			int i1 = face[0] + offset;
			int i2 = face[1] + offset;
			int i3 = face[2] + offset;

			Shape *triangle = new Triangle(i1, i2, i3);
			primitives.push_back(PrimitiveInfo(triangle, vertices[i1].position, vertices[i2].position, vertices[i3].position));

			if (smooth) ComputeNormal(vertices, i1, i2, i3);
		}
	}
	else
	{
		for (std::vector<int> face : fInd)
		{
			int i1 = face[0] + offset;
			int i2 = face[1] + offset;
			int i3 = face[2] + offset;
			int i4 = face[3] + offset;

			Shape * triangle = new Triangle(i1, i2, i3);
			primitives.push_back(PrimitiveInfo(triangle, vertices[i1].position, vertices[i2].position, vertices[i3].position));
			Shape * triangle1 = new Triangle(i3, i4, i1);
			primitives.push_back(PrimitiveInfo(triangle1, vertices[i3].position, vertices[i4].position, vertices[i1].position));

			if (smooth)
			{
				ComputeNormal(vertices, i1, i2, i3);
				ComputeNormal(vertices, i3, i4, i1);
			}
		}
	}

}

Ray Scene::RoughReflectionRay(const Vector3 & pos, const Vector3 & dir, real roughness, real time)
{
	Vector3 u, v;
	dir.OrthonormalBasis(u, v);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rng(0.0, 1.0);

	Vector3 roughDir = ((u * (rng(gen) - 0.5) + v * (rng(gen) - 0.5)) * roughness + dir).Unit();

	return Ray(pos, roughDir, time);
}



void Scene::ComputeNormal(std::vector<Vertex> &vertices, int i1, int i2, int i3)
{
	Vector3 e1 = vertices.at(i2).position - vertices.at(i1).position;
	Vector3 e2 = vertices.at(i3).position - vertices.at(i1).position;

	Vector3 normal = (e1 % e2).Unit();

	vertices.at(i1).normal += normal;
	vertices.at(i2).normal += normal;
	vertices.at(i3).normal += normal;
}


void Scene::Photon_Tracing(std::vector<Photon*> &photons, const Ray &photonRay, Vector3 power, int flag, int depth, int caustic)
{
	//if (depth <= 0) return;

	RayHit * rayHit = new RayHit();

	for (ObjectInfo* object : objects)
	{
		object->Intersect(photonRay, primitives, vertices, rayHit);
	}

	if (rayHit->t == REAL_MAX)
	{
		delete rayHit;
		return;
	}

	Material material = *(materials.at(rayHit->matIndex));

	if (caustic && !flag && material.type != MaterialType::Dielectric) return;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rng(0, 1);

	if (material.type == MaterialType::None)
	{
		real ref_probability = std::max(material.diffuseRef.x, std::max(material.diffuseRef.y, material.diffuseRef.z));
		power *= material.diffuseRef;

		if (rng(gen) > ref_probability)
		{

			Photon * absorbed = new Photon();
			absorbed->position = rayHit->pos;
			absorbed->direction = rayHit->toEye;
			absorbed->power = power / (1 - ref_probability);

			photons.push_back(absorbed);

		}
		else
		{
			Vector3 direction;
			Vector3 u, v;

			rayHit->normal.OrthonormalBasis(u, v);

			real s2 = rng(gen);
			real sqr_s2 = real_sqrt(s2);
			real sqr_one_minus_s2 = real_sqrt(1 - s2);
			real s1 = rng(gen) * 2 * R_PI;

			direction = u * sqr_s2 * real_cos(s1) + rayHit->normal * sqr_one_minus_s2 + v * sqr_s2 * real_sin(s1);

			direction.Normalise();

			Ray diffusely_reflected_ray(rayHit->pos + rayHit->normal * 0.001, direction, photonRay.time);

			Photon_Tracing(photons, diffusely_reflected_ray, power * (1.0 / ref_probability), 1, depth - 1, caustic);
		}

	}
	else if (material.type == MaterialType::Mirror)
	{
		Vector3 ref_dir = photonRay.direction - rayHit->normal * 2 * (photonRay.direction * rayHit->normal);
		ref_dir.Normalise();

		Ray specular_reflected_ray(rayHit->pos + rayHit->normal * 0.001, ref_dir, photonRay.time);

		Photon_Tracing(photons, specular_reflected_ray, power, 1, depth - 1, caustic);
	}
	else if (material.type == MaterialType::Dielectric)
	{
		Vector3 wo = rayHit->toEye;
		real fresnel;
		Vector3 wt, wr;
		Vector3 n = rayHit->normal;
		real etaI = 1;
		real etaT = material.refractionIndex;

		real cosI = wo * n;

		bool leaving = cosI < real(0);
		if (leaving)
		{
			std::swap(etaI, etaT);
			n = -n;
			cosI = real_abs(cosI);
		}

		real eta = etaI / etaT;

		real sin2I = std::max(real(0), 1 - cosI * cosI);
		real sin2T = eta * eta * sin2I;

		if (sin2T >= 1)
		{
			fresnel = 1;
		}
		else
		{
			real cosT = real_sqrt(1 - sin2T);

			real parallel = (etaT * cosI - etaI * cosT) / (etaT * cosI + etaI * cosT);
			real perpendicular = (etaI * cosI - etaT * cosT) / (etaI * cosI + etaT * cosT);

			fresnel = (parallel * parallel + perpendicular * perpendicular) / 2;
			wt = wo * -eta + n * (eta * cosI - cosT);
			wt.Normalise();
		}

		wr = -wo + n * cosI * 2;
		wr.Normalise();

		real probability = 0.25 + 0.5 * fresnel;

		if (rng(gen) > fresnel)
		{
			Ray refraction(rayHit->pos - n * 0.001, wt, photonRay.time);
			Photon_Tracing(photons, refraction, power, 1, depth - 1, caustic);
		}
		else
		{
			Ray reflection(rayHit->pos + n * 0.001, wr, photonRay.time);
			Photon_Tracing(photons, reflection, power, 1, depth - 1, caustic);
		}

	}

	delete rayHit;
}

void Scene::Build_Photon_Map()
{
	std::vector<Photon*> global_map_photons;
	std::vector<Photon*> caustic_photons;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rng(0, 1);

	Vector3 power = Vector3(1, 1, 1);

	for (Light * l : lights)
	{
		AreaLight * area_l = dynamic_cast<AreaLight *>(l);
		if (area_l)
		{
			int n_photons = area_l->intensity.x * 4 * R_PI;
			while (n_photons--)
			{
				Vector3 light_pos = area_l->position + area_l->u * (rng(gen) - 0.5) * area_l->size + area_l->v * (rng(gen) - 0.5) * area_l->size;
				real s2 = rng(gen);
				real sqr_s2 = real_sqrt(s2);
				real sqr_one_minus_s2 = real_sqrt(1 - s2);
				real s1 = rng(gen) * 2 * R_PI;

				Vector3 light_dir = area_l->u * sqr_s2 * real_cos(s1) + area_l->direction * sqr_one_minus_s2 + area_l->v * sqr_s2 * real_sin(s1);
				light_dir.Normalise();

				Ray photon_ray(light_pos, light_dir, rng(gen));

				Photon_Tracing(global_map_photons, photon_ray, power, 0, 10, 0);
				Photon_Tracing(caustic_photons, photon_ray, power, 0, 10, 1);
			}
		}
		else
		{
			printf("Photon emission is supported from only area lights\n");
		}
	}

	global_photon_map = new PhotonMap(global_map_photons);
	caustic_photon_map = new PhotonMap(caustic_photons);
}

Vector3 Scene::Radiance(const Ray& ray, int depth)
{
	if (depth <= 0) return Vector3();

	RayHit * rayHit = new RayHit();

	for (ObjectInfo * object : objects)
	{
		object->Intersect(ray, primitives, vertices, rayHit);
	}

	if (rayHit->t == REAL_MAX)
	{
		delete rayHit;
		return Vector3();
	}

	Material material = *(materials.at(rayHit->matIndex));

	if (material.type == MaterialType::None)
	{
		delete rayHit;
		return global_photon_map->Radiance_Estimate(rayHit->pos, rayHit->normal, 0.4, 100);
	}
	else if (material.type == MaterialType::Mirror)
	{
		Vector3 ref_dir = ray.direction - rayHit->normal * 2 * (ray.direction * rayHit->normal);
		ref_dir.Normalise();

		Ray specular_reflected_ray(rayHit->pos + rayHit->normal * 0.001, ref_dir, ray.time);

		delete rayHit;
		return Radiance(specular_reflected_ray, depth - 1);
	}
	else if (material.type == MaterialType::Dielectric)
	{
		Vector3 wo = rayHit->toEye;
		real fresnel;
		Vector3 wt, wr;
		Vector3 n = rayHit->normal;
		real etaI = 1;
		real etaT = material.refractionIndex;

		real cosI = wo * n;

		bool leaving = cosI < real(0);
		if (leaving)
		{
			std::swap(etaI, etaT);
			n = -n;
			cosI = real_abs(cosI);
		}

		real eta = etaI / etaT;

		real sin2I = std::max(real(0), 1 - cosI * cosI);
		real sin2T = eta * eta * sin2I;

		if (sin2T >= 1)
		{
			fresnel = 1;
		}
		else
		{
			real cosT = real_sqrt(1 - sin2T);

			real parallel = (etaT * cosI - etaI * cosT) / (etaT * cosI + etaI * cosT);
			real perpendicular = (etaI * cosI - etaT * cosT) / (etaI * cosI + etaT * cosT);

			fresnel = (parallel * parallel + perpendicular * perpendicular) / 2;
			wt = wo * -eta + n * (eta * cosI - cosT);
			wt.Normalise();
		}

		wr = -wo + n * cosI * 2;
		wr.Normalise();


		Ray refraction(rayHit->pos - n * 0.001, wt, ray.time);


		Ray reflection(rayHit->pos + n * 0.001, wr, ray.time);

		delete rayHit;

		return Radiance(reflection, depth - 1) * fresnel + Radiance(refraction, depth - 1) * (1 - fresnel);
	}

}


void Scene::PrintSceneElementCount()
{
	std::cout << "maxRecursionDepth" << std::endl;
	std::cout << maxRecursionDepth << std::endl;
	std::cout << "intTestEps" << std::endl;
	std::cout << intTestEps << std::endl;
	std::cout << "shadowRayEps" << std::endl;
	std::cout << shadowRayEps << std::endl;
	std::cout << "backgroundColor" << std::endl;
	std::cout << backgroundColor << std::endl;
	std::cout << "ambientLight" << std::endl;
	std::cout << ambientLight << std::endl;

	std::cout << "cameras" << std::endl;
	std::cout << cameras.size() << std::endl;

	std::cout << "lights" << std::endl;
	std::cout << lights.size() << std::endl;

	std::cout << "materials" << std::endl;
	std::cout << materials.size() << std::endl;

	std::cout << "vertices" << std::endl;
	std::cout << vertices.size() << std::endl;

	std::cout << "objects" << std::endl;
	std::cout << objects.size() << std::endl;

	std::cout << "textures" << std::endl;
	std::cout << textures.size() << std::endl;

	std::cout << "primitives" << std::endl;
	std::cout << primitives.size() << std::endl;

}
