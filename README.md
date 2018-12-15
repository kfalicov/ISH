# Live Site: https://paulh2019.github.io/ISH/
## Follow progress at https://twitter.com/ISH_Capstone
WIP game using SDL2 for CS425 at GMU, Game Programming. Senior capstone project.

by Paul Hughes and Kyle Falicov

## Gameplay

In the Shadow of a Hero (ISH) is an RPG about the atrocities committed by the protagonists of other video games. Your task is to venture into the world and help those in need, those who have been conned by the so-called "hero".

In ISH, you will explore a vast procedurally generated world, interact with the locals (including not-so-friendly wildlife), and right the wrongs of the hero before you. Tread carefully, as decisions you make when interacting with the world may influence future events. Townspeople will remember if you wrong them and spread the word, while being kind may just aggravate your new friends' enemies.

## Technical Components

ISH is made with SDL2 and the FastNoise library (https://github.com/Auburns/FastNoise). We have implemented the following technologies:

#### Render Layers

ISH's main loop includes a stack of states, which are all updated 30 times per second. Rendering is done as fast as possible.

Each state of our game has its own surface that it renders to. Rather than constantly re-rendering every time Render() is called, each state updates its surface as needed. If nothing visually changes in a state, it will simply return the previously rendered surface. The game then combines the surfaces of each state into a single surface, which is then shown on screen.

##### Notes

Our original implementation of rendering simply rendered everything visible on the screen each time Render() was called. Each state rendered directly to the surface that was going to be cast to the screen. Obviously, this was horrendously inflexible and slow, and ran at ~20fps for a simple scene.

Post-refactor (as described above), we have gained much needed flexibility, and we can render each layer in whichever order we please. Additionally, our performance increased ~50x, and now runs at ~1000fps for an equivalent scene.

#### Asset Handling

Whenever an image needs to be loaded for use in a sprite, the system first checks if the image has already been loaded. If it has, it just returns the existing spritesheet pointer. Otherwise, it will load the image from the specified file. This allows multiple sprites to all share a single spritesheet, and the spritesheet does not have to be reloaded for each sprite that uses it, greatly reducing the memory footprint.

#### Sprite Animations

Sprite animations consist of a set of rectangles that each represent a single frame of the animation, as well as a pointer to the spritesheet used in the animation. When a sprite with animation is created, it is added to a list of animated sprites. The sprite frames and animation changes all occur in unison for visual appeal. To increment the frame that a given animation is on, we just use the next rectangle in the list as the source rect of the spritesheet, or loop back to the first one if needed.

Using this system, an entity can have multiple animations from either a single spritesheet or many spritesheets. Entities can then queue up their animation change, which will occur when the animation systems updates all animations simultaneously.
