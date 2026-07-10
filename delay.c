 //-----------------------------------------------------------
 // Function : delay_us()
 // Purpose  : Generate an approximate delay in microseconds
 // Input    : tdly - Delay time in microseconds
 //-----------------------------------------------------------
void delay_us(unsigned int tdly)
{
    // Convert microseconds into loop count
    // Assuming a 12 MHz clock (1 µs ≈ 12 CPU cycles)
    tdly *= 12;

    // Busy-wait loop
    while(tdly--);
}

//-----------------------------------------------------------
// Function : delay_ms()
// Purpose  : Generate an approximate delay in milliseconds
// Input    : tdly - Delay time in milliseconds
//-----------------------------------------------------------
void delay_ms(unsigned int tdly)
{
    // Convert milliseconds into loop count
    // 1 ms = 1000 µs
    // 1000 × 12 = 12000 loop iterations
    tdly *= 12000;

    // Busy-wait loop
    while(tdly--);
}

//-----------------------------------------------------------
// Function : delay_s()
// Purpose  : Generate an approximate delay in seconds
// Input    : tdly - Delay time in seconds
//-----------------------------------------------------------
void delay_s(unsigned int tdly)
{
    // Convert seconds into loop count
    // 1 second = 1,000,000 µs
    // 1,000,000 × 12 = 12,000,000 loop iterations
    tdly *= 12000000;

    // Busy-wait loop
    while(tdly--);
}
