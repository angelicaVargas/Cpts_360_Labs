/*
 * Titan Lander
 *
 * Adapted from Dave Ahl and Joe Morrison's classic "Lunar Lander".
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>  // get prototypes of strtod() and getenv()
#include <getopt.h>  // get getopt()
#include <assert.h>  // get assert()
#include "set_default_double.c"


#define MAX_THRUST_DEFAULT 5.0
#define INITIAL_HEIGHT_DEFAULT 1000.0
#define INITIAL_VELOCITY_DEFAULT -50.0 // downwards
#define INITIAL_FUEL_DEFAULT 100.0
#define GRAVITY_DEFAULT 1.352 // titan surface gravity in m/s**2

// Create global struct
struct {
    double maxThrust;
    double initialFuel;
    double gravity;
    double initialHeight;
    double initialVelocity;
} parameters;


double getThrust(double fuel)
/*
 * Prompt the user to enter a legal thrust value. Engines cannot
 * proved more than a (given) maximum thrust value. Allow for errors.
 *
 * input:
 *   fuel: available fuel
 *
 * returns:
 *   a legal thrust value
 */
{
    double thrust, maxBurn;

    maxBurn = parameters.maxThrust;
    if (fuel < parameters.maxThrust)
        maxBurn = fuel;

    printf("thrust (0 - %.1f): ", maxBurn);
    scanf("%lf", &thrust);
    if (thrust < 0) {
        printf("thrust cannot be negative -- assuming zero\n");
        thrust = 0.0;
    } else if (thrust > maxBurn) {
        printf("maximum burn is %f -- clamping to that\n", maxBurn);
        thrust = maxBurn;
    }
    return thrust;
}

void step(double thrust, double *height_p, double *velocity_p,
          double *fuel_p, double *elapsedTime_p)
/*
 * Compute the change in height, velocity, fuel, and elapsed time for
 * a single time step. Optional: If the lander crashes, allow for the
 * fact that it does not sink into the ground.
 *
 * input:
 *
 *   thrust: amount of thrust to apply (taken from fuel)
 *
 * inputs/outputs:
 *
 *   height_p: (pointer to) height above the ground (in m.)
 *     before/after step
 *
 *   velocity_p: (pointer to) velocity (in m.) (negative is downward)
 *
 *   fuel_p: (pointer to) amount of fuel available/remaining
 *
 *   elapsedTime_p: (pointer to) clock time before/after step (in s.)
 */
{
    double dt, accel;
    double newVelocity;
    double newHeight;
    double gravity = parameters.gravity;

    /*
     * This is the time step. The only time it changes is if the
     * lander crashes.
     */
    dt = 1.0;

    accel         = thrust - gravity; // negative in free fall
    newVelocity   = (*velocity_p) + accel * dt;
    newHeight     = (*height_p) + (*velocity_p)*dt + 0.5*accel*dt*dt;
    (*fuel_p) -= thrust;
    if (newHeight >= 0) {
        (*elapsedTime_p) += dt;
        (*velocity_p) = newVelocity;    
        (*height_p) = newHeight;
    } else {
        /*
         * The lander has crashed, so figure out the time of impact.
         */
        if (accel == 0) {
            dt = (*height_p) / (*velocity_p);
        } else {
            dt = (-(*velocity_p)
                - sqrt((*velocity_p)*(*velocity_p) - 2*accel*(*height_p)))
                / accel;
        }
        (*elapsedTime_p) += dt;
        (*velocity_p) += accel * dt;
        (*height_p) = 0.0;
    }
}

void land(double *finalVelocity_p, double *elapsedTime_p)
/*
 * Simulate the landing.
 *
 * input:
 *
 *    fuel: the initial amount of fuel in thrust units
 *
 * outputs:
 *
 *    finalVelocity_p: (pointer to) the velocity of the lander upon
 *      landing (in meters per second)
 *
 *    elapsedTime_p: (pointer to) the total time required to land (in
 *      seconds)
 *
 */
{
    double height        = parameters.initialHeight;
    double velocity      = parameters.initialVelocity;
    double fuel          = parameters.initialFuel;
    double thrust; 

    (*elapsedTime_p) = 0.0;
    while (height > 0.0) {
        printf("           time: %6.2f s\n",    (*elapsedTime_p));
        printf("         height: %6.1f m\n",       height);
        printf("       velocity: %6.1f m/s\n", velocity);
        printf(" remaining fuel: %6.1f units\n",  fuel);
        if (fuel > 0) {
            thrust = getThrust(fuel);
        } else {
            printf("    OUT OF FUEL: FREE FALLING\n");
            thrust = 0;
        }
        step(thrust, &height, &velocity, &fuel, elapsedTime_p);
        printf("\n");
	}
    (*finalVelocity_p) = velocity;
    return;
}

int main(int argc, char *argv[])
{
    double elapsedTime;
    double finalVelocity;
    int optionLetter;

    parameters.maxThrust = setDefaultDouble("MAX_THRUST", MAX_THRUST_DEFAULT);
    parameters.initialFuel = setDefaultDouble("INITIAL_FUEL", INITIAL_FUEL_DEFAULT);
    parameters.gravity = setDefaultDouble("GRAVITY", GRAVITY_DEFAULT);
    parameters.initialHeight = setDefaultDouble("INITIAL_HEIGHT", INITIAL_HEIGHT_DEFAULT);
    parameters.initialVelocity = setDefaultDouble("INITIAL_VELOCITY", INITIAL_VELOCITY_DEFAULT);

    while ((optionLetter = getopt(argc, argv, "t: f: g: h: v:")) != -1) {
        switch (optionLetter) {
            
        case 't':
            parameters.maxThrust = strtod(optarg, NULL);
            break;
        case 'f':
            parameters.initialFuel = strtod(optarg, NULL);
            break;
        case 'g':
            parameters.gravity = strtod(optarg, NULL);
            break;
        case 'h':
            parameters.initialHeight = strtod(optarg, NULL);
            break;
        case 'v':
            parameters.initialVelocity = strtod(optarg, NULL);
            break;
        default:
            assert(0); // just in case
        }
    }

    land(&finalVelocity, &elapsedTime);
    printf("final velocity: %.1f m/s\n", finalVelocity);
    if (fabs(finalVelocity) > 2.0)
        printf("you crashed!\n");
    else
        printf("safe landing\n");
    printf("  elapsed time: %.2f s\n", elapsedTime);
    return 0;
}