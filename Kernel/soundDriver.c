#include<soundDriver.h>

void play_sound(unsigned int frequency) {
    // Input validation
    if (frequency < MIN_FREQUENCY || frequency > MAX_FREQUENCY) {
        return; // Invalid frequency, do nothing
    }
    
    unsigned int divisor;
    unsigned char tmp;
    
    // Calculate divisor for the PIT to generate the desired frequency
    divisor = PIT_FREQUENCY / frequency;
    
    // Prevent division by zero or invalid divisor
    if (divisor == 0 || divisor > 0xFFFF) {
        return;
    }
    
    // Configure PIT channel 2 in square wave mode, 16-bit (0xB6 = 10110110b)
    outb(PIT_COMMAND_PORT, 0xB6);
    
    // Send divisor to PIT channel 2 
    outb(PIT_CHANNEL2_PORT, (unsigned char)(divisor & 0xFF));        // Low byte
    outb(PIT_CHANNEL2_PORT, (unsigned char)((divisor >> 8) & 0xFF)); // High byte
    
    // Enable PC speaker by setting bits 0 and 1 in speaker control port
    tmp = inb(SPEAKER_CONTROL_PORT);
    outb(SPEAKER_CONTROL_PORT, tmp | 0x03); // Set bits 0 and 1
}

void stopSound(void) {
    unsigned char tmp;
    
    // Disable PC speaker by clearing bits 0 and 1 in speaker control port  
    tmp = inb(SPEAKER_CONTROL_PORT);
    outb(SPEAKER_CONTROL_PORT, tmp & 0xFC); // Clear bits 0 and 1 (0xFC = 11111100b)
}

void beep(unsigned int frequency, unsigned int duration_ms) {
    play_sound(frequency);
 
    // Simple delay loop (this is platform/timing dependent)
    // In a real kernel, you'd use proper timer interrupts
    for (volatile unsigned int i = 0; i < duration_ms * 1000; i++) {
        // Busy wait - not ideal but simple
    }
    
    stopSound();
}