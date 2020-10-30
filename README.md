# ray-tracing
CPU-based ray tracer.

Included features and techniques
---
* Basic Shading and Shadows
* BRDFs
	* Phong
	* Modified Phong
	* Normalized Modified Phong
	* Blinn-Phong
	* Modified Blinn-Phong
	* Normalized Modified Blinn-Phong
	* Torrance-Sparrow
* Lighting
	* Virtual Lights
		* Point Light
		* Spot Light
		* Directional Light
		* Area Light
		* Environment Light
	* Object Lights
		* Mesh Light
		* Sphere Light
* Recursive Ray Tracing
	* Reflection
	* Refraction
* Acceleration Structure
	* Bounding Volume Hierarchy
* Transformations and Instancing
* Multisampling
* Smooth Shading
* Distribution Ray Tracing
	* Motion Blur
	* Glossy Reflection
	* Depth of Field
* Textures
	* Image Textures
	* Procedural Textures (Perlin noise)
* Texture Mapping	
* Normal Mapping
* Bump Mapping
* HDR Imaging
* Tone Mapping
* Output Image Formats
	* Png
	* Jpg
	* Exr
* Multithreading
* Path Tracing
* Photon Mapping

Rendered Scenes
---
|  |  | |
:-------------------------:|:-------------------------: |:-------------------------:
![](https://1.bp.blogspot.com/-RAZpzJFjJiA/XvsQRvHdkUI/AAAAAAAAAMQ/8wuy8ls62706nqYMWMPfTMtwoS5X_3LbgCPcBGAYYCw/s1600/diffuse_next_importance.png) | ![](https://1.bp.blogspot.com/-PQP9PW52d2g/XvsQnPYejeI/AAAAAAAAAM4/jEXnrhqe4XYeo04p8UrizjHSNdRXZ3u-QCPcBGAYYCw/s1600/glass_next_russian.png) | ![](https://1.bp.blogspot.com/-HqBDHgEMUz4/XvsQEOrpjmI/AAAAAAAAAL8/IY7rQfFOULUpdeElW65lMjHsWqONYBJeACPcBGAYYCw/s1600/cornellbox_jaroslav_glossy_area_sphere.png) | 
![](https://1.bp.blogspot.com/-ezRIBy1jxJ4/XvsQBdFLjUI/AAAAAAAAALg/8X2oRwi5ZRAELo60JGf3BIAivccH45SFwCPcBGAYYCw/s1600/cornellbox_jaroslav_glossy_area.png) | ![](https://1.bp.blogspot.com/-rvlaTGvRIeI/XvsQDD_r7rI/AAAAAAAAALs/_MTpE-fNHl0Z4Zan8y6Jv7FdSVNzPrkYgCPcBGAYYCw/s1600/cornellbox_jaroslav_glossy_area_ellipsoid.png) | ![](https://1.bp.blogspot.com/-ugmHeBCiVEg/XvsQDc4_yaI/AAAAAAAAAMQ/dnT4AtnYjiA-sOEr_XSXaPc7E6iXQH2IACPcBGAYYCw/s1600/cornellbox_jaroslav_glossy_area_small.png) | 
![](https://1.bp.blogspot.com/-qvbqI8UtjC4/XsrYSCasKbI/AAAAAAAAAI0/3jGiYCfR91I8Rk9v6fV4B3-TsnJAD-aqwCPcBGAYYCw/s1600/dragon_spot_light_msaa.png) | ![](https://1.bp.blogspot.com/-r67J7o9PQZ4/XtlkXuH5fdI/AAAAAAAAAKI/FhwNBtKcmisRFljL_4-cNanNcratcWE-wCPcBGAYYCw/s1600/killeroo_blinnphong.png) | ![](https://1.bp.blogspot.com/-6DL49ZHesmU/XsrYfA3q6_I/AAAAAAAAAI4/bUupYwqo4Qo76miN0uNyBMQReMk99m-7QCPcBGAYYCw/s1600/sphere_point_hdr_texture.png) | 
![](https://1.bp.blogspot.com/-3a0j5_vjF9E/XtlkfEFMgDI/AAAAAAAAAKQ/pxCkoNUO9KgrXDJxr7HVTUjmNHaHyiyCQCPcBGAYYCw/s1600/killeroo_torrancesparrow_closeup.png) | ![](https://1.bp.blogspot.com/-r6YSG7eR5mk/Xq9j_fqBMQI/AAAAAAAAAHk/OjyzPtizurYFbPH-P7clhuG-Z4UTrTSwQCPcBGAYYCw/s1600/sphere_perlin_scale.png) | ![](https://1.bp.blogspot.com/-cE_fSqtD6m0/Xq9j9hKcHKI/AAAAAAAAAHY/PbibA7wpyV0sixN95KGmj3f-LHKF3s_4ACPcBGAYYCw/s1600/sphere_normal.png) | 
![](https://1.bp.blogspot.com/-E5EvSQ23OJs/Xq9j88ClGJI/AAAAAAAAAHU/jNOklONgeEUmKcNGMLscMMbMRCkWRd4JwCPcBGAYYCw/s1600/sphere_nobump_bump.png) | ![](https://1.bp.blogspot.com/-kzsztvcO-ak/XpLcqK4PaxI/AAAAAAAAAEQ/zSnxY1JwpUAaacxu_M08UXRPgi7C6FJdACPcBGAYYCw/s1600/cornellbox_brushed_metal.png) | ![](https://1.bp.blogspot.com/-UE9I82kC1k0/XpLcrK4C7WI/AAAAAAAAAEc/cAwQ5cgH2ogNurpQLWeQ-g95ncp81I7awCPcBGAYYCw/s1600/metal_glass_plates.png) | 
![](https://1.bp.blogspot.com/-ftWLEaUG-Gc/XpLcqRAVHgI/AAAAAAAAAD4/iybNSCTXd-U7GLXYRIuov2KjFFJMtCY_QCLcBGAsYHQ/s1600/cornellbox_boxes_dynamic.png) | ![](https://1.bp.blogspot.com/-0DFpQ0AsKAU/XpLcrhEj9MI/AAAAAAAAAEY/78w7LLo7etwSXeXUMsSSv2P04qk85tuVACPcBGAYYCw/s1600/spheres_dof.png) | ![](https://1.bp.blogspot.com/-dTukfsFUVnw/XmRKjxYoyDI/AAAAAAAAADc/T9DeEc4rh8M3_Q9nNFAOPdhbL2GTE0aQwCEwYBhgL/s1600/chinese_dragon.png) | 
![](https://1.bp.blogspot.com/-yvMaytRarOg/XlEwXJhRp4I/AAAAAAAAACE/nN8P6wluL8cBXnYHg14vac25cYw6SXJ0gCLcBGAsYHQ/s1600/bunny.png) | ![](https://1.bp.blogspot.com/-c_9Azws-ZAE/XlEwgJL5wVI/AAAAAAAAACY/cbnG0iL8GucXxUjShdaFXOrAxDm3po9vQCLcBGAsYHQ/s1600/dragon_lowres.png) | ![](https://1.bp.blogspot.com/-fd0GAj8PukI/XmRKyqCtnAI/AAAAAAAAADY/4cYBWCXFrt40pk2ls7_naHuWvIK5H1k4ACEwYBhgL/s1600/spheres.png) | 

||||
:-------------------------:|:-------------------------: |:-------------------------:
![](https://1.bp.blogspot.com/-xU3YU_xR4hE/Xq9j6vKw72I/AAAAAAAAAHk/9ibakg1wxkQ9xkznKgl3Y-8J-elFNn8NwCPcBGAYYCw/s1600/galactica_dynamic.png) | ![](https://1.bp.blogspot.com/-uxICz1R-MVE/XmRK1XlaljI/AAAAAAAAADg/44KJT8jZa0UE3DbtbYzAv880_jyj4hv_gCEwYBhgL/s1600/horse_and_mug.png) | ![](https://1.bp.blogspot.com/-pXasey8ZKDM/XsrYeQwmQmI/AAAAAAAAAI0/sBr7Dgskf9caOeqIOP72L1VIoP7dbVQlgCPcBGAYYCw/s1600/head_env_light_bracket.png) |







