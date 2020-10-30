src = *.cpp
src2 = */*.cpp

build:
	nvcc $(src) $(src2) -lpthread -std=c++14 -O3 -o raytracer
	
part1:
	./raytracer assets/scenes/bunny.xml
	./raytracer assets/scenes/cornellbox.xml
	./raytracer assets/scenes/scienceTree_old.xml
	./raytracer assets/scenes/simple.xml
	./raytracer assets/scenes/spheres_old.xml

part2:
	./raytracer assets/scenes/chinese_dragon.xml
	./raytracer assets/scenes/cornellbox_recursive.xml
	./raytracer assets/scenes/other_dragon.xml
	./raytracer assets/scenes/scienceTree.xml
	./raytracer assets/scenes/spheres.xml
	
part3:
	./raytracer assets/scenes/cornellbox_boxes_dynamic.xml
	./raytracer assets/scenes/cornellbox_brushed_metal.xml
	./raytracer assets/scenes/dragon_dynamic.xml
	./raytracer assets/scenes/metal_glass_plates.xml
	./raytracer assets/scenes/simple_transform.xml
	./raytracer assets/scenes/spheres_dof.xml
	./raytracer assets/scenes/tap_0200.xml
	./raytracer assets/scenes/ellipsoids.xml
	
part4:
	./raytracer assets/scenes/bump_mapping_transformed.xml
	./raytracer assets/scenes/cube_cushion.xml
	./raytracer assets/scenes/cube_perlin.xml
	./raytracer assets/scenes/cube_perlin_bump.xml
	./raytracer assets/scenes/cube_wall.xml
	./raytracer assets/scenes/cube_wall_normal.xml
	./raytracer assets/scenes/cube_waves.xml
	./raytracer assets/scenes/ellipsoids_texture.xml
	./raytracer assets/scenes/sphere_nearest_bilinear.xml
	./raytracer assets/scenes/sphere_nobump_bump.xml
	./raytracer assets/scenes/sphere_nobump_justbump.xml
	./raytracer assets/scenes/sphere_normal.xml
	./raytracer assets/scenes/sphere_perlin.xml
	./raytracer assets/scenes/sphere_perlin_bump.xml
	./raytracer assets/scenes/sphere_perlin_scale.xml
	./raytracer assets/scenes/killeroo_bump_walls.xml
	./raytracer assets/scenes/galactica_static.xml
	./raytracer assets/scenes/galactica_dynamic.xml
	
part5:
	./raytracer assets/scenes/cornellbox_area.xml
	./raytracer assets/scenes/cube_directional.xml
	./raytracer assets/scenes/cube_point.xml
	./raytracer assets/scenes/cube_point_hdr.xml
	./raytracer assets/scenes/dragon_spot_light_msaa.xml
	./raytracer assets/scenes/sphere_point_hdr_texture.xml
	./raytracer assets/scenes/head_env_light.xml
	
part6:
	./raytracer assets/scenes/brdf_blinnphong_modified.xml
	./raytracer assets/scenes/brdf_blinnphong_modified_normalized.xml
	./raytracer assets/scenes/brdf_blinnphong_original.xml
	./raytracer assets/scenes/brdf_phong_modified.xml
	./raytracer assets/scenes/brdf_phong_modified_normalized.xml
	./raytracer assets/scenes/brdf_phong_original.xml
	./raytracer assets/scenes/brdf_torrancesparrow.xml
	./raytracer assets/scenes/killeroo_blinnphong.xml
	./raytracer assets/scenes/killeroo_torrancesparrow.xml
	
other:
	./raytracer assets/scenes/dragon_lowres.xml
	./raytracer assets/scenes/horse_and_mug.xml
	
part7:
	./raytracer assets/scenes/cornellbox_jaroslav_diffuse.xml
	./raytracer assets/scenes/cornellbox_jaroslav_diffuse_area.xml
	./raytracer assets/scenes/cornellbox_jaroslav_glossy.xml
	./raytracer assets/scenes/cornellbox_jaroslav_glossy_area.xml
	./raytracer assets/scenes/cornellbox_jaroslav_glossy_area_small.xml
	./raytracer assets/scenes/cornellbox_jaroslav_glossy_area_sphere.xml
	./raytracer assets/scenes/cornellbox_jaroslav_glossy_area_ellipsoid.xml
	./raytracer assets/scenes/cornellbox_jaroslav_path_diffuse.xml
	./raytracer assets/scenes/cornellbox_jaroslav_path_glass.xml
	
all:
	make other
	make part1
	make part2
	make part3
	make part4
	make part5
	make part6
	make part7
	


