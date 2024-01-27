# Serial 
> protocol serial RSCUAD

## How to use
- recommended

        // just include rscuad.h
        
        // in setup
        RSCUAD::Get()->serial->Init(1);

        // in loop
        RSCUAD::Get()->serial->Process()
        RSCUAD::Get()->serial->Gyro();
        RSCUAD::Get()->serial->Button();
        

- not recommended

        // in setup
        Serial serial;      // initial object

        serial.Init()       // use old method, too slow
        serial.Init(1)      // for more faster | but not tested yet
        
        // in loop
        serial.Gyro();      // data gyro int
        serial.Button();    // data button int
        serial.Process();   // call in loop

## Data sending

    // gyro[space]button
    
    data = "360 1";