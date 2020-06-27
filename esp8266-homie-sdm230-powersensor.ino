#include <Homie.h>
HomieNode powerMonitorNode("pwrmon", "PowerMonitor", "power-monitor");

#include <timeouter.h>
timeouter waitPublish;

#include <SDM.h>
#include <SoftwareSerial.h> //import SoftwareSerial library
#define DERE_PIN  12  //for control DE/RE lines (connect DE & /RE together to this pin)
SoftwareSerial swSerSDM;    //config SoftwareSerial
SDM sdm(swSerSDM, 9600, DERE_PIN, SWSERIAL_8N1, SDM_RX_PIN, SDM_TX_PIN); //config SDM

/*
bool somHandler(const HomieRange& range, const String& value) {
  for (byte i = 0; i < value.length(); i++) {
    if (isDigit(value.charAt(i)) == false) return false;
  }

  const float som = value.toInt();
  if (som > 10) return false;

  waitPublish.setDelay((1/som)*120000);

  return true;
}
*/

void loopHandler() {
  float val = 0.0;
  // work with all sensors and transmit data
  if (waitPublish.isOver()) {  
    val = sdm.readVal(SDM230_VOLTAGE);
    Homie.getLogger() << "Voltage: " << val << " V" << endl;
    powerMonitorNode.setProperty("voltage").send(String(val));  
    
    val = sdm.readVal(SDM230_CURRENT);
    Homie.getLogger() << "Current: " << val << " A" << endl;
    powerMonitorNode.setProperty("current").send(String(val));
    
    val = sdm.readVal(SDM230_POWER);
    Homie.getLogger() << "Power: " << val << " W" << endl;
    powerMonitorNode.setProperty("power").send(String(val));      
    
    val = sdm.readVal(SDM230_FREQUENCY);
    Homie.getLogger() << "Frequency: " << val << " Hz" << endl;
    powerMonitorNode.setProperty("frequency").send(String(val));  
    
    val = sdm.readVal(SDM230_TOTAL_ACTIVE_ENERGY);
    Homie.getLogger() << "Total active energy: " << val << " kWh" << endl;
    powerMonitorNode.setProperty("total_active_energy").send(String(val));

    val = sdm.readVal(SDM230_TOTAL_REACTIVE_ENERGY);
    Homie.getLogger() << "Total reactive energy: " << val << " kVarh" << endl;
    powerMonitorNode.setProperty("total_reactive_energy").send(String(val));    

    val = sdm.readVal(SDM230_PHASE_ANGLE);
    Homie.getLogger() << "Phase angle: " << val << " °" << endl;
    powerMonitorNode.setProperty("phase_angle").send(String(val));  

    waitPublish.start();
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("sdm230-power-sensor", "1.0.2");
  Homie.setLoopFunction(loopHandler);
    
  waitPublish.setDelay(120000);
  waitPublish.start();

  sdm.begin(); //initialize SDM communication
 
  /*
  powerMonitorNode.advertise("som"    ).setName("Speed of measurements")
                                       .setDatatype("integer")
                                       .setFormat("0:10")
                                       .settable(somHandler);
  */                                     
  
  powerMonitorNode.advertise("voltage").setName("Voltage")
                                       .setDatatype("float")
                                       .setUnit("V");
  
  powerMonitorNode.advertise("current").setName("Current")
                                       .setDatatype("float")
                                       .setUnit("A");
  
  powerMonitorNode.advertise("power"  ).setName("Power")
                                       .setDatatype("float")
                                       .setUnit("W");
  
  powerMonitorNode.advertise("frequency").setName("Frequency")
                                        .setDatatype("float")
                                        .setUnit("Hz");
  
  powerMonitorNode.advertise("total_active_energy").setName("Total active energy")
                                        .setDatatype("float")
                                        .setUnit("kWh");

  powerMonitorNode.advertise("total_reactive_energy").setName("Total reactive energy")
                                        .setDatatype("float")
                                        .setUnit("kVarh");                                        
  
  powerMonitorNode.advertise("phase_angle").setName("Phase angle")
                                        .setDatatype("float")
                                        .setUnit("°");                                         

  Homie.setup();
}

void loop() {
  Homie.loop();
}
