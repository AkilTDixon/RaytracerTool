
# Raytracer Tool

A comprehensive C++ raytracing engine that generates images through lighting and rendering techniques. This project demonstrates both local and global illumination models with support for various geometric primitives and lighting configurations.

## Features

### 🎨 **Advanced Rendering Techniques**
- **Local Illumination**: Traditional Phong shading with ambient, diffuse, and specular components
- **Global Illumination**: Monte Carlo path tracing with configurable bounce limits and probability termination
- **Anti-aliasing**: Stratified sampling for smooth edges and reduced noise
- **Multi-threading**: Parallel rendering for improved performance

### 🔧 **Geometric Primitives**
- **Spheres**: Full sphere intersection with proper normal calculations
- **Rectangles**: Planar surfaces with four-point definition
- **Extensible Architecture**: Easy to add new geometric shapes

### 💡 **Lighting System**
- **Point Lights**: Omnidirectional light sources
- **Area Lights**: Extended light sources for soft shadows
- **Multiple Light Support**: Handle multiple light sources in a single scene
- **Shadow Casting**: Accurate shadow calculations with proper visibility testing

### 📷 **Camera System**
- **Configurable FOV**: Field of view control for different perspectives
- **Look-at Camera**: Position, target, and up vector specification
- **Aspect Ratio Support**: Proper image aspect ratio handling
- **Background Colors**: Customizable background rendering

### ⚙️ **Configuration & Performance**
- **JSON Scene Files**: Human-readable scene configuration
- **Configurable Sampling**: Adjustable rays per pixel for quality vs. performance
- **Thread Management**: User-configurable thread count for parallel processing
- **PPM Output**: Standard image format output

## Example Results

<img width="500" height="500" alt="Example 3C (Local Illumination)" src="https://github.com/user-attachments/assets/ef996428-861f-4ed7-be9f-2ce9caa2ccf0" />
<img width="500" height="500" alt="Example 3CG (Global Illumination)" src="https://github.com/user-attachments/assets/bad45717-7ff5-423c-8ef8-36b5bf3f8f5f" />

**Left: Local Illumination | Right: Global Illumination**

Notice how global illumination creates realistic color bleeding (red from the floor affecting the white sphere) and softer, more natural shadows compared to the hard shadows in local illumination.

## Scene Configuration

Scenes are defined using JSON files with the following structure:

```json
{
  "geometry": [
    {
      "type": "sphere",
      "centre": [0, 0, -4],
      "radius": 1,
      "ac": [1, 1, 1],  // Ambient color
      "dc": [1, 1, 1],  // Diffuse color  
      "sc": [1, 1, 1],  // Specular color
      "ka": 0,          // Ambient coefficient
      "kd": 1,          // Diffuse coefficient
      "ks": 1,          // Specular coefficient
      "pc": 100         // Phong coefficient
    }
  ],
  "light": [
    {
      "type": "point",
      "centre": [-1, 7, -5],
      "id": [1, 1, 1],  // Diffuse intensity
      "is": [1, 1, 1]   // Specular intensity
    }
  ],
  "output": [
    {
      "filename": "output.ppm",
      "size": [500, 500],
      "centre": [0, 0.5, 0],
      "lookat": [0, 0, -1],
      "up": [0, 1, 0],
      "fov": 60,
      "globalillum": true,
      "raysperpixel": [10, 10],
      "maxbounces": 3,
      "probterminate": 0.333
    }
  ]
}
```

## Building and Running

### Prerequisites
- Visual Studio 2019 or later (Windows)
- C++17 compatible compiler
- Eigen3 library (included)

### Build Instructions
1. Open `Raytracer.sln` in Visual Studio
2. Select your preferred configuration (Debug/Release, x64/x86)
3. Build the solution (Ctrl+Shift+B)

### Usage
1. Place your scene JSON files in the `Raytracer/assets/` directory
2. Run the executable
3. Enter the scene filename (without .json extension) when prompted
4. Specify the number of threads for parallel processing
5. The rendered image will be saved as a PPM file

## Technical Implementation

### Core Components
- **RayTracer**: Main rendering engine with scene parsing and thread management
- **Geometry**: Base class for all renderable objects with intersection testing
- **Light**: Lighting calculations including shadow testing and recursive bouncing
- **Camera**: View matrix calculations and ray generation
- **Helper**: Utility functions for mathematical operations and color handling

### Key Algorithms
- **Ray-Sphere Intersection**: Quadratic formula for sphere intersection
- **Ray-Plane Intersection**: Barycentric coordinates for rectangle intersection
- **Monte Carlo Integration**: Random sampling for global illumination
- **Phong Shading Model**: Realistic surface lighting calculations
- **Stratified Sampling**: Anti-aliasing through sub-pixel sampling

### Performance Optimizations
- **Z-sorting**: Objects sorted by depth for early termination
- **Multi-threading**: Parallel pixel processing with work distribution
- **Early Ray Termination**: Probability-based path termination in global illumination
- **Efficient Data Structures**: Optimized geometry and light storage

## Project Structure

```
RaytracerTool/
├── Raytracer/                 # Main source code
│   ├── assets/               # Scene configuration files
│   ├── Eigen/                # Linear algebra library
│   ├── *.cpp, *.h            # Core implementation files
│   └── Raytracer.vcxproj     # Visual Studio project file
├── examples/                 # Sample rendered images
└── README.md                 # This file
```

## Dependencies

- **Eigen3**: Linear algebra and matrix operations
- **nlohmann/json**: JSON parsing and manipulation
- **Standard C++17**: Modern C++ features and threading support



## License

This project is developed for educational purposes. Please ensure proper attribution when using or modifying the code.

---



