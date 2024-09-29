# MSimul

**MSimul** is a custom-built physics simulator inspired by the famous physics-chemistry simulator, Powder Toy. Developed from scratch, it incorporates advanced physical simulations with unique features such as movable solids and interactions, such as a water can, while supporting complex scenarios like water pouring, fire, and TNT explosions.

## Features

- **Water Physics**: Simulates realistic water behavior, including pouring and pressure effects.
  
  ![Water Physics](https://github.com/MihneaAndreescu/MSimul/blob/main/8F0C4B4A-E15F-4524-8A3E-AC52EE972695.png)

- **Solid Elements like iron, wood, and diamonds**

  ![Solid Elements](https://github.com/MihneaAndreescu/MSimul/blob/main/1F8E8088-7BEC-4B21-BB1C-E40019739537.png)

- **Fire and Smoke**: Dynamic simulation of fire that can ignite wood and other materials, producing smoke.
  
  ![Fire and Smoke](https://github.com/MihneaAndreescu/MSimul/blob/main/4F502D51-F277-48FE-855E-4CD5BCC00B52.png)

- **Explosions**: TNT can be detonated, with pressure waves impacting surrounding elements.

  ![Explosions](https://github.com/MihneaAndreescu/MSimul/blob/main/DAF6C789-45D9-463C-AA15-CB42F5A15A4B.png)

- **Steam Generation**: Water transforms into steam upon contact with heat sources like fire or heated surfaces.
- **World Saving and Loading**: You can save and load custom world configurations for replay or continued experimentation.

## Available Elements

- **Water**: Fluid simulation with pressure and flow effects.
- **Iron**: A solid element, which can interact with other materials and heat.
- **Acid**: Reacts with certain materials, causing degradation.
- **Diamond**: A tough, indestructible solid.
- **Steam**: Created when water is heated by fire or lava.
- **Wood**: A flammable solid material that burns when in contact with fire.
- **Fire**: Burns flammable materials and produces smoke.
- **Smoke**: Produced from burning elements like wood or TNT.
- **TNT**: Can be detonated, causing explosive effects and generating pressure waves.

## How to Play

- **Simulation Control**: Move elements around and observe how they interact in real-time.
- **Save/Load**: Create custom worlds and save them for later. Load previously saved worlds to resume from where you left off.

## Project Structure

- **Main.cpp**: The main file where the core logic and game loop are implemented.
- **ParticlePhysicsTableSingleton.cpp**: Contains the physics logic for the particle simulations, including interactions like water flow, fire, and explosions.
- **RngSingleton.cpp**: Implements the random number generator used for various randomized simulations, like fire spread and explosions.

## Example Usage

- Pour water over wood, ignite it with fire, and watch the smoke rise.
- Set off a TNT explosion and observe how the shockwave interacts with nearby objects.
- Simulate water pressure by pouring water into different containers.

