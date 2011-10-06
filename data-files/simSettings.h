/* Don't define SIM_ON_USER_INPUT and SIM_AUTO at the same time. */

// the image file to use as the initial environment texture
#define START_FILE "water-ball.png"

// SIM_ON_USER_INPUT means simulation is only run while the user is holding the space bar
//#define SIM_ON_USER_INPUT

// SIM_AUTO means the simulation is run automatically
#define SIM_AUTO

// SIM_TIME is the sim-world time that will pass between each simulation step
#define SIM_TIME 0.1

// SIM_SLICES is the number of pieces the simulation is divided into
#define SIM_SLICES 4

/* Don't define SIM_TEST and RENDER at the same time */

// SIM_TEST renders in debug mode where every material attribute is visible and each cell is labeled
// probably unwise to use with a large environment view
//#define SIM_TEST

// RENDER renders normally
#define RENDER

#define SHOW_PLAYER
#define SHOW_HUD

#define ICON_SIZE 50

// Useful constants
#define DEPTH (255.0)
#define DEPRC (1.0 / DEPTH)


// simulation automata constants (the letter in front indicate the 
// channel that uses the constant)
#define R_BORDER 255
#define R_AIR 0
#define R_SAND 1
#define R_WATER 2
#define R_WOOD 3
#define R_FIRE 4
#define R_SMOKE 5
#define G_STILL 0
#define G_FALLING 1
#define G_INSUB 2
#define G_FLOW_RIGHT 3
#define G_FLOW_LEFT 4
#define G_RISING 5
#define MAX_WATER 5
#define FIRE_TIME 5

// only spawn on empty cells
#define SPAWN_MODE self.r == R_AIR

// spawn on any cell
//#define SPAWN_MODE true

//Water properties
#define MAX_MASS 100 //The normal, un-pressurized mass of a full water cell
#define MAX_COMPRESS 2 //How much excess water a cell can store, compared to the cell above it
#define MIN_MASS 3  //Ignore cells that are almost dry
#define VISCOSITY 4 // Restricts the flow of water
#define MAX_FLOW 5
#define MAX_REFRACTION (10.0) // The maximum number of pixels to refract by (float)

// Maximum time (in passes) a particle of smoke can live. Must be less than 255
#define SMOKE_LIFE 200

// Render constants
#define FIRE_DEPTH 5
#define CAMERA_HOR_VEL 600
#define CAMERA_VERT_VEL 600

// Player constants
#define PLAYER_HOR_VEL 150
#define PLAYER_VERT_VEL 450
#define GRAVITY 15
#define RANGE_SAND 10.0f
#define RANGE_WATER 10.0f
#define RANGE_FIRE 80.0f
#define RANGE_WOOD 15.0f
#define MIN_PLACEMENT_FACTOR 1.05

// facing
#define FACE_RIGHT 0
#define FACE_LEFT 1
