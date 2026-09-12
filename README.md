# QuadTree Image Compression

Lossy image compression using a **QuadTree data structure** with adaptive spatial subdivision.

The project was developed as a course project for the **Algorithms and Data Structures** discipline. The main goal was to investigate how a QuadTree can be used to represent an image with controllable quality loss.

## Overview

A QuadTree recursively divides a two-dimensional region into four subregions.

For image compression, the algorithm analyzes each region of the image and calculates:

1. the average RGB color of the region;
2. the average Euclidean color error between the pixels and the average color.

If the average error is greater than a specified `THRESHOLD`, the region is divided into four quadrants and processed recursively.

If the error is within the threshold, the region is represented by a single color — its average color.

This produces an adaptive representation: areas with relatively uniform colors require fewer nodes, while detailed areas are subdivided more deeply.

```text
                         Image
                           │
                    calculate average
                       region color
                           │
                  calculate average error
                           │
                 ┌─────────┴─────────┐
                 │                   │
          error <= THRESHOLD    error > THRESHOLD
                 │                   │
                 ▼                   ▼
          store one color       split into 4
          for the region       subregions
                                     │
                      ┌──────────────┼──────────────┐
                      ▼              ▼              ▼              ▼
                   Top-Left       Top-Right     Bottom-Left   Bottom-Right
                      │              │              │              │
                      └──────────────┴──────────────┴──────────────┘
                                     │
                                recurse
```

## Algorithm

### Image → QuadTree

The image is first converted into a square whose side length is a power of two. This makes recursive subdivision into four equal regions straightforward.

For each region:

```text
1. Calculate the average RGB color.
2. Calculate the average Euclidean distance
   between every pixel and the average color.
3. Compare the error with THRESHOLD.

   error <= THRESHOLD
       → create a leaf node representing the region

   error > THRESHOLD
       → divide the region into four quadrants
       → recursively process each quadrant
```

The color distance is calculated in RGB space:

$$
d(C_1,C_2)=
\sqrt{(R_1-R_2)^2+(G_1-G_2)^2+(B_1-B_2)^2}
$$

The average error for a region is:

$$
E=\frac{1}{N}\sum_{i=1}^{N}d(C_i,C_{avg})
$$

where:

* \(N\) is the number of pixels in the region;
* \(C_i\) is the color of a pixel;
* \(C_{avg}\) is the average color of the region.

The resulting QuadTree stores the average color in each node and recursively stores four child nodes when further subdivision is required.

### QuadTree → Image

The decompression process traverses the tree recursively.

If a node has no children, its color is used to reconstruct the corresponding image region.

If a node has children, the algorithm recursively processes its four quadrants.

```text
QuadTree
   │
   ├── leaf
   │     └── fill region with node color
   │
   └── internal node
         ├── top-left
         ├── top-right
         ├── bottom-left
         └── bottom-right
                    │
                  recurse
```

This reconstructs an approximation of the original image according to the selected `THRESHOLD`.

## Examples

Only a few examples are included here to keep the README compact.

### Original image

<img width="699" height="524" alt="image" src="https://github.com/user-attachments/assets/20744449-9a07-4ae4-8e81-fb75172c25ef" />


### Reconstruction with different thresholds

#### Threshold = 20

<img width="698" height="523" alt="image" src="https://github.com/user-attachments/assets/bcbb2010-14f6-426a-86d6-1436ca3461b1" />

#### Threshold = 40

<img width="698" height="523" alt="image" src="https://github.com/user-attachments/assets/9fbf45f2-cbbd-4285-9a9a-7d64524c12bb" />

Higher `THRESHOLD` values allow more regions to be represented by a single color, resulting in stronger compression and greater visual distortion.

## Data Structure

The main data structure is a QuadTree node.

Conceptually, each node contains:

```text
Node
├── color
├── topLeft
├── topRight
├── bottomLeft
└── bottomRight
```

A **leaf node** represents a homogeneous-enough image region using a single average color.

An **internal node** represents a region that requires further subdivision and contains four child nodes.

This structure allows the representation to adapt to the spatial complexity of the image.

## Technologies

* **C++**
* **Qt types** — `QImage`, `QColor`, `QSize`, `QPoint`
* Recursive algorithms
* QuadTree data structure
* RGB color representation

Qt is used primarily for image handling and several utility/data types. The core of the project is the **algorithm and data structure implementation**, rather than Qt application development.

## Project Background

This project was developed as a university course project for the **Algorithms and Data Structures** discipline in 2023.

The task was to investigate an approach to efficient image representation with possible quality loss using a QuadTree.

The project combines implementation of the data structure with an experimental evaluation of how the compression threshold affects the resulting representation and reconstruction error.

## Conclusion

The project demonstrates an adaptive lossy image representation based on a QuadTree.

Instead of applying the same level of detail to the entire image, the algorithm recursively subdivides only those regions whose color variation exceeds the selected threshold. This allows the resulting representation to preserve more detail in visually complex areas while using a coarser representation for relatively uniform regions.

The experimental results show the expected relationship between the `THRESHOLD`, representation size, and reconstruction error, illustrating the main trade-off of the proposed approach.
