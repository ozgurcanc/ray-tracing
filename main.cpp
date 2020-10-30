#include "renderer/Scene.h"

int main(int argc, char *argv[])
{	

	if (argc != 2)
    {
        std::cout << "Please run the ray tracer as:" << std::endl
             << "\t./raytracer assets/scenes/<input_file_name>" << std::endl;
        return 1;
    }

	const char *xmlPath = argv[1];

	Scene * s = new Scene();

	s->CreateScene(xmlPath);

    s->RenderScene();
	
	return 0;
}
