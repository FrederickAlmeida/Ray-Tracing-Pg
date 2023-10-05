# Ray-Tracing-Pg

Project from the course of "Graphics Processing" at the Federal University of Pernambuco (UFPE).
Recursirve Ray Tracing implementation in C++. Using Phong's model for shading and reflection and refraction effects.
Extra feature: Texture mapping into plane object.

## Group

- Beatriz Freire
- Caio Possidio
- Felipe Neiva
- Frederick Almeida
- Hugo Marques
- Ian Braga
- Rubens Lima

## Usage

1. Compile the main file with the command:
```bash
g++ main.cpp -o main.exe 

```

2. Create your .txt input file with the camera, objects, lights and ambient light information. The file must be in the following format:

```bash	
# camera: 
c image_width image_height fov up_x up_y up_z eye_x eye_y eye_z center_x center_y center_z max_recursion_level

# sphere:
s center_x center_y center_z radius r g b kd ks ka kr kt rugosity refraction_index

# triangle (for each vertice, put its coordinates and for each face, put the index of its vertices ):

t num_faces num_vertices 
vi_x vi_y vi_z 
vi+1_x vi+1_y vi+1_z 
fi_firstvertice fi_secondvertice fi_thirdvertice 
fi+1_firstvertice fi+1_secondvertice fi+1_thirdvertice
r g b kd ks ka kr kt rugosity refraction_index

# plane:

p point_x point_y point_z normal_x normal_y normal_z r g b kd ks ka kr kt rugosity refraction_index

# plane with texture (choose the texture of folder 'textures' in the render.cpp file):

i point_x point_y point_z normal_x normal_y normal_z r g b kd ks ka kr kt rugosity refraction_index

# ambient light:

a r g b

# light source:

l light_x light_y light_z r g b

```

3. Run the main file with the command:

```bash
./main.exe  < "inputs/file.txt" >> output.ppm

```

4. Open the output.ppm file with a ppm viewer.

