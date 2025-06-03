#ifndef SOUND_DRIVER_H
#define SOUND_DRIVER_H

// Hardware constants
#define PIT_FREQUENCY 1193180      // Base frequency of the PIT in Hz
#define PIT_COMMAND_PORT 0x43      // PIT command register
#define PIT_CHANNEL2_PORT 0x42     // PIT channel 2 data port
#define SPEAKER_CONTROL_PORT 0x61  // PC speaker control port

// Frequency limits for safety
#define MIN_FREQUENCY 20           
#define MAX_FREQUENCY 20000       

// Function declarations with port I/O (these should be defined elsewhere)
extern void outb(unsigned short port, unsigned char value);
extern unsigned char inb(unsigned short port);

/**
 * Plays a sound at the specified frequency using the PC speaker
 * @param frequency: Frequency in Hz (20-20000 Hz recommended)
 */
void play_sound(unsigned int frequency);

/**
 * Stops the currently playing sound by disabling the PC speaker
 */
void stopSound(void);

/**
 * Plays a beep for a specified duration
 * @param frequency: Frequency in Hz (20-20000 Hz recommended)
 * @param duration_ms: Duration in milliseconds
 */
void beep(unsigned int frequency, unsigned int duration_ms);

#endif // SOUND_DRIVER_H