//#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>             // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>             // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <lib_dmx.h>         // deskontrol four universes DMX library
#include "artnet_node.h"
#include "common.h"          // definitions from libartnet
#include "packets.h"         // headers from libartnet, striped version

//*********************************************************************************************************

//Encoder myEnc(2, 3);
//long oldPosition  = -999;

// OLED Display Config.
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define bojit_GLCD_HEIGHT  32 
#define bojit_GLCD_WIDTH  128 

static const unsigned char PROGMEM bojit_glcd_bmp[] =
 // 'BOJIT Bitmap',
{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xe0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xce, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xc4, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xe6, 0x00, 0x01, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xc6, 0x0f, 0xc1, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xcf, 0xff, 0xe3, 0xc0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xcf, 0xff, 0xc3, 0x80, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xc9, 0xfb, 0xc3, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xce, 0x19, 0xf3, 0xc3, 0x67, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xe6, 0x39, 0xfb, 0xc3, 0xe7, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xbf, 0xc7, 0xe7, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x7c, 0x07, 0xc7, 0xe7, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x30, 0x03, 0x87, 0xe7, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 0x87, 0xe7, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x80, 0x41, 0x87, 0xcf, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xc0, 0xe1, 0x8f, 0xcf, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xc1, 0xe1, 0x8f, 0xcf, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xc0, 0xe1, 0x0f, 0xcf, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x80, 0x01, 0x0e, 0x01, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 0x1e, 0x01, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x78, 0x07, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xfc, 0x0e, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xc0, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
//************************************************************************************************************
//                        New DMX modes
#define    DMX512     (0)    // (250 kbaud - 2 to 512 channels) Standard USITT DMX-512

#define   USE_UNIVERSE_0         // remember universe 0 now is USART 0, Arduino output pin 1

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) && !defined(USE_TINKERKIT_SHIELD)
	#define   USE_UNIVERSE_1      // remember universe 1 now is USART 1, Arduino output pin 18

#endif

uint8_t factory_mac          [6] = {   1,   2,   3,   0,   0,  10}; // the mac address of node
uint8_t factory_localIp      [4] = {   192 ,   168,   1,  50};           // the IP address of node
uint8_t factory_broadcastIp  [4] = {   192, 255, 255, 255};           // broadcast IP address
uint8_t factory_gateway      [4] = {   192,   168,   1,   2};           // gateway IP address (use ip address of controller)
uint8_t factory_subnetMask   [4] = { 255,   0,   0,   0};           // network mask (art-net use 'A' network type)

uint8_t factory_swin         [4] = {   0,   1,   2,   3};
uint8_t factory_swout        [4] = {   0,   1,   2,   3};

int tx_ch, tx_mode;

artnet_node_t             ArtNode;
artnet_reply_t            ArtPollReply;
artnet_packet_type_t      packet_type;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  const int MAX_BUFFER_UDP = 1650;	// For Arduino MEGA
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  const int MAX_BUFFER_UDP = 550;	// For Arduino UNO, due to only have 2kb of RAM, and only 1 UART
#endif                

uint8_t packetBuffer [MAX_BUFFER_UDP];             //buffer to store incoming data (volatile?)

EthernetUDP Udp;

void setup() 
{
 // pinMode(4, INPUT);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done

  //Set Display Invert
  display.invertDisplay(true);
  display.clearDisplay();  
  // miniature bitmap display
  display.drawBitmap(0, 0, bojit_glcd_bmp, 128 , 32 , 1);
  display.display();
  delay(3000);
  display.clearDisplay();
    
  display.invertDisplay(false);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20,5);
  display.print("IP: ");
  display.print("192.168.1.50");
  display.setCursor(10,20);
  display.print("Uni. 1:");
  display.print(" =");
  display.setCursor(70,20);
  display.print("Uni. 2:");
  display.print(" =");

  fill_art_node   (&ArtNode);
    
  ArtNode.numbports  = 0;

#if defined(USE_UNIVERSE_0)
  ArduinoDmx0.set_control_pin(7);     // max485 input/output control (connect to 485 pins 2-3) 
  ArduinoDmx0.set_tx_address(1);       // set tx0 start address
  tx_ch = 512;                       // number of TX channels - Defined by mode selected below
  
  ArduinoDmx0.set_tx_channels(tx_ch);
  ArduinoDmx0.init_tx(DMX512);        // starts universe 0 as tx

  ArtNode.numbports ++;
#endif

#if defined(USE_UNIVERSE_1)
  ArduinoDmx1.set_control_pin(6);     // max485 input/output control (connect to 485 pins 2-3) 
  ArduinoDmx1.set_tx_address(1);       // set tx1 start address
  tx_ch = 512;                       // number of TX channels - Defined by mode selected below
  tx_mode = DMX512;                    // define DMX mode; DMX512: standard DMX512, DMX1024: non-standard DMX1024, DMX2048: non-standard DMX2048

  ArduinoDmx1.set_tx_channels(tx_ch);
  ArduinoDmx1.init_tx(DMX512);        // starts universe 1 as tx 
  
  ArtNode.numbports ++;
#endif

  display.display();

  // subnet mask needed because art-net uses 'A' type network (255.0.0.0), ethernet library defaults to 255.255.255.0
  Ethernet.begin(ArtNode.mac, ArtNode.localIp, ArtNode.localIp, ArtNode.gateway, ArtNode.subnetMask); 
  Udp.begin(ArtNode.localPort);
  
  send_reply(BROADCAST, (uint8_t *)&ArtPollReply, sizeof(ArtPollReply));

}

void loop() 
{
 /*   long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    display.clearDisplay();
    display.setCursor(20,5);
    display.print(newPosition);
    display.display();
  }*/
  if(Udp.parsePacket() > ARNET_HEADER_SIZE ) 
  { 
    handle_packet();
  }
}

void handle_packet()
{
  Udp.read((uint8_t *)&packetBuffer, MAX_BUFFER_UDP/*, (uint8_t *)&ArtNode.remoteIp, (uint16_t *)&ArtNode.remotePort*/);  
  
  packet_type = (artnet_packet_type_t) get_packet_type((uint8_t *)&packetBuffer);

  if(packet_type == 0)  // bad packet
  {
    return;
  }  
  if(packet_type == ARTNET_DMX)
  {
    if(sizeof(packetBuffer) < sizeof(artnet_dmx_t)) 
      return;
    else
      handle_dmx((artnet_dmx_t *)&packetBuffer); 
  }   
  else if(packet_type == ARTNET_POLL)
  { 
    if(sizeof(packetBuffer) < sizeof(artnet_poll_t)) 
      return;
    else
      handle_poll((artnet_poll_t *)&packetBuffer);
  } 
  
  else if(packet_type == ARTNET_ADDRESS)
  {
    if(sizeof(packetBuffer) < sizeof(artnet_address_t))
      return;
    else
      handle_address((artnet_address_t *)&packetBuffer);
  } 
}

uint16_t get_packet_type(uint8_t *packet) //this get artnet packet type
{
  if (! memcmp( packet, ArtNode.id, 8)) 
  {
    return bytes_to_short(packet[9], packet[8]); 
  } 
  return 0;  // bad packet
}

int handle_dmx(artnet_dmx_t *packet)
{
  if(packet->universe == ArtNode.swout[0])
  {   
    #if defined(USE_UNIVERSE_0)
      memcpy ((uint8_t *)ArduinoDmx0.TxBuffer, (uint8_t *)packet->data, ARTNET_DMX_LENGTH);
    #endif
  }
  else if(packet->universe == ArtNode.swout[1])
  {   
    #if defined(USE_UNIVERSE_1)
      memcpy ((uint8_t *)ArduinoDmx1.TxBuffer, (uint8_t *)packet->data, ARTNET_DMX_LENGTH);
    #endif
  }
}

int handle_poll(artnet_poll_t *packet) 
{
  if((packet->ttm & 1) == 1) // controller say: send unicast reply
  {
    send_reply(UNICAST, (uint8_t *)&ArtPollReply, sizeof(ArtPollReply));
  }
  else // controller say: send broadcast reply
  {
    send_reply(BROADCAST, (uint8_t *)&ArtPollReply, sizeof(ArtPollReply));
  }
}

int handle_address(artnet_address_t *packet) //not implemented yet
{
  send_reply(UNICAST, (uint8_t *)&ArtPollReply, sizeof(ArtPollReply));
}

void send_reply(uint8_t mode_broadcast, uint8_t *packet, uint16_t size)
{
  if(mode_broadcast == 1) // send broadcast packet
  {
    Udp.beginPacket(ArtNode.broadcastIp, ArtNode.remotePort);
  }
  else // send unicast packet to controller
  {
    Udp.beginPacket(ArtNode.remoteIp, ArtNode.remotePort);
  }
  Udp.write(packet, size);
  Udp.endPacket();
}

void fill_art_node(artnet_node_t *node)
{
  //fill to 0's
  memset (node, 0, sizeof(node));
  
  //fill data
  memcpy (node->mac, factory_mac, 6);                   // the mac address of node
  memcpy (node->localIp, factory_localIp, 4);           // the IP address of node
  memcpy (node->broadcastIp, factory_broadcastIp, 4);   // broadcast IP address
  memcpy (node->gateway, factory_gateway, 4);           // gateway IP address
  memcpy (node->subnetMask, factory_subnetMask, 4);     // network mask (art-net use 'A' network type)
  
  sprintf((char *)node->id, "Art-Net\0"); // *** don't change never ***
  sprintf((char *)node->shortname, "BOJIT node\0");
  sprintf((char *)node->longname, "Art-Net Node BOJIT");
    
  memset (node->porttypes,  0x80, 4);
  memset (node->goodinput,  0x08, 4);
  //memset (node->goodoutput, 0x00, 4);
  
    
  node->subH           = 0x00;        // high byte of the Node Subnet Address (This field is currently unused and set to zero. It is
                                      // provided to allow future expansion.) (art-net III)
  node->sub            = 0x00;        // low byte of the Node Subnet Address
  
  // **************************** art-net address of universes **********************************
  node->swout      [0] = 0x00;        // This array defines the 8 bit Universe address of the available output channels.
  node->swout      [1] = 0x01;        // values from 0x00 to 0xFF
  node->swout      [2] = 0x02;
  node->swout      [3] = 0x03;
  
  // not implemented yet
  node->swin       [0] = 0x00;        // This array defines the 8 bit Universe address of the available input channels.
  node->swin       [1] = 0x01;        // values from 0x00 to 0xFF
  node->swin       [2] = 0x02;
  node->swin       [3] = 0x03;
  
#if defined(USE_UNIVERSE_0)
  node->goodoutput [0] = 0x80;
#endif

#if defined(USE_UNIVERSE_1)
  node->goodoutput [1] = 0x80;
#endif

  node->etsaman[0] = 0;        // The ESTA manufacturer code.
  node->etsaman[1] = 0;        // The ESTA manufacturer code.
  node->localPort  = 0x1936;   // artnet UDP port is by default 6454 (0x1936)
  node->verH       = 0;        // high byte of Node firmware revision number.
  node->ver        = 2;        // low byte of Node firmware revision number.
  node->ProVerH    = 0;        // high byte of the Art-Net protocol revision number.
  node->ProVer     = 14;       // low byte of the Art-Net protocol revision number.
  node->oemH       = 0;        // high byte of the oem value.
  node->oem        = 0xFF;     // low byte of the oem value. (0x00FF = developer code)
  node->ubea       = 0;        // This field contains the firmware version of the User Bios Extension Area (UBEA). 0 if not used
  node->status     = 0x08;
  node->swvideo    = 0;
  node->swmacro    = 0;
  node->swremote   = 0;
  node->style      = 0;        // StNode style - A DMX to/from Art-Net device
}

void fill_art_poll_reply(artnet_reply_t *poll_reply, artnet_node_t *node)
{
  //fill to 0's
  memset (poll_reply, 0, sizeof(poll_reply));
  
  //copy data from node
  memcpy (poll_reply->id, node->id, sizeof(poll_reply->id));
  memcpy (poll_reply->ip, node->localIp, sizeof(poll_reply->ip));
  memcpy (poll_reply->mac, node->mac, sizeof(poll_reply->mac)); 
  memcpy (poll_reply->shortname, node->shortname, sizeof(poll_reply->shortname)); 
  memcpy (poll_reply->longname, node->longname, sizeof(poll_reply->longname));
  memcpy (poll_reply->nodereport, node->nodereport, sizeof(poll_reply->mac));
  memcpy (poll_reply->porttypes, node->porttypes, sizeof(poll_reply->porttypes));
  memcpy (poll_reply->goodinput, node->goodinput, sizeof(poll_reply->goodinput));
  memcpy (poll_reply->goodoutput, node->goodoutput, sizeof(poll_reply->goodoutput));
  memcpy (poll_reply->swin, node->swin, sizeof(poll_reply->swin));
  memcpy (poll_reply->swout, node->swout, sizeof(poll_reply->swout));
  memcpy (poll_reply->etsaman, node->etsaman, sizeof(poll_reply->etsaman));
  
  sprintf((char *)poll_reply->nodereport, "%i DMX output universes active.\0", node->numbports);
  
  poll_reply->opCode          = 0x2100;  // ARTNET_REPLY
  poll_reply->port            = node->localPort;
  poll_reply->verH            = node->verH;
  poll_reply->ver             = node->ver;
  poll_reply->subH            = node->subH;
  poll_reply->sub             = node->sub;
  poll_reply->oemH            = node->oemH;
  poll_reply->oem             = node->oem;
  poll_reply->status          = node->status;
  poll_reply->numbportsH      = node->numbportsH;
  poll_reply->numbports       = node->numbports;
  poll_reply->swvideo         = node->swvideo;
  poll_reply->swmacro         = node->swmacro;
  poll_reply->swremote        = node->swremote;
  poll_reply->style           = node->style; 
}


