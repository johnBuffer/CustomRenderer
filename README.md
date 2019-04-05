# CustomRenderer
SFML Render helper allowing to define custom rendering pipelines

# Installation
Include header
```cpp
#include "Renderer.hpp"
```

# Usage

## Basic usage

### 1. Create a renderer
```cpp
Renderer renderer(win_width, win_height);
```

### 2. Draw something with it
```cpp
// Create shape
const float r(100.0f);
sf::CircleShape shape(r);
shape.setFillColor(sf::Color::Green);
shape.setOrigin(r, r);

// Draw the shape
renderer.draw(shape);
```

### 3. Display the result
```cpp
window.draw(renderer.render());
window.display();
```

![bloom_img](https://github.com/johnBuffer/CustomRenderer/blob/master/img/simple_draw.png)

## Adanced usage with example: _Bloom effect_

### 1. Create a custom render stage inheriting from the _RenderStage_ base class

This class defines what need to be done to apply the bloom effect. 

Here I am using my [Blur](https://github.com/johnBuffer/DynamicBlur) library.

```cpp
#include "Renderer.hpp"
#include "dynamic_blur.hpp"

class BloomStage : public RenderStage
{
public:
  BloomStage(ID tex1, ID tex2, uint32_t width, uint32_t height) :
    RenderStage(tex1, tex2),
    m_blur(width, height)
  {}

  // The method that will be called 
  void process(sf::RenderTexture& bloom_texure, sf::RenderTexture& final_render) const
  {
    bloom_texure.display();
    final_render.draw(sf::Sprite(m_blur.apply(bloom_texure.getTexture(), 4)), sf::BlendAdd);
  }
  
  using BloomStagePtr = std::shared_ptr<RenderStage>;
  static BloomStagePtr create(ID tex1, ID tex2, uint32_t width, uint32_t height)
  {
    return std::make_shared<BloomStage>(tex1, tex2, width, height);
  }

private:
  // Blur lib
  mutable Blur m_blur;
};
```

### 2. Add the stage to the renderer pipeline
```cpp
// Create a new layer
const ID bloom_texture(renderer.addLayer());

// Add the stage to the render pipeline
renderer.getPipeline().addStage(BloomStage::create(bloom_texture, Renderer::FinalTexture, win_width, win_height));
```

### 3. Draw the glowing shape on the appropriate layers
```cpp
// Draw the shape
renderer.draw(shape);

// Notify it's glowing by drawing it on the bloom layer
renderer.draw(shape, bloom_texture);
```

### 4. Display the result
```cpp
window.draw(renderer.render());
window.display();
```

![bloom_img](https://github.com/johnBuffer/CustomRenderer/blob/master/img/bloom_draw.png)











