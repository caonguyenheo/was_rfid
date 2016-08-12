#include "ros/ros.h"
#include <ros/xmlrpc_manager.h>
//#include <std_msgs/Bool.h>
//#include <std_msgs/Byte.h>
#include <std_msgs/Char.h>
//#include <std_msgs/Duration.h>
//#include <std_msgs/Header.h>
//#include <std_msgs/Byte.h>
#include <std_msgs/UInt32.h>
#include <std_msgs/UInt8.h>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/UInt16MultiArray.h"
#include "std_msgs/UInt8MultiArray.h"
#include "BlackSPI.h"
#include "BlackGPIO.h"
#include "BlackCore.h"
#include "BlackErr.h"
#include "BlackDef.h"
#include <sstream>
#include <iostream>
#include "std_msgs/String.h"
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <array>
using namespace std;
void protocol();
int  inventory();
uint8_t NFCReady = 0;
uint8_t readIN[500];
uint8_t i, k,len,tam;
int main(int argc, char **argv)
{
        ros::init(argc, argv, "ws_rfid");
        ros::NodeHandle n;
//        ros::Publisher chatter_pub = n.advertise<std_msgs::Char>("was_rfid/movement/rfid", 1000);
//	 ros::Publisher chatter_pub = n.advertise<std_msgs::UInt8>("was_rfid/movement/rfid", 1000);
//	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("was_rfid/movement/rfid", 1000);
        ros::Publisher chatter_pub = n.advertise<std_msgs::UInt8MultiArray>("was_rfid/movement/rfid", 1000);
        ros::Rate loop_rate(1);

	BlackLib::BlackGPIO set1(BlackLib::GPIO_49, BlackLib::output);
	
        sleep(1);

	set1.setValue(BlackLib::high);
	usleep(500);

	set1.setValue(BlackLib::low);
	usleep(100);

	set1.setValue(BlackLib::high);
	sleep(1);
	protocol();
	sleep(1);
//	inventory();
	/**********************************************SET PROTOCOL************************************************/
	/********************************************INVENTORY***************************************************/
	// std_msgs::UInt16MultiArray msg;
//	 std_msgs::UInt16MultiArray ms;
	//uint32_t s2[500];
	uint8_t ms;
	uint8_t j;
//	char h[15];
//	uint8_t ptr[14];
//	ptr = j;
	std_msgs::UInt8MultiArray msg;
//	 std_msgs::UInt8 msg;
//	std_msgs::Char msg;
//	std_msgs::String msg;
        while (ros::ok())
	 {	
		msg.data.clear();
      		inventory();
		ms=readIN[2];
//		for(j=1;j<ms+2;j++)
		if(k==1)
		{
			for(j=1;j<ms+2;j++)
			{
			//	printf("%02X:",readIN[j]);
//	 msg.data=(readIN[1]&&readIN[2]&&readIN[3]&&readIN[4]&&readIN[5]&&readIN[6]&&readIN[7]&&readIN[8]&&readIN[9]&&readIN[10]&&readIN[11]&&readIN[12]&&readIN[13]&&readIN[14]);
//				puts(h);				//uint8_t dem=(uint8_t) h[15];
			//	printf("%02X",dem);
//				msg.data=readIN[j];
//				std::stringstream ss;
			//	ss=stringstream(h);
//				ss<"i"<h[15i];
//				msg.data = ss.str();
//				chatter_pub.publish(msg);
//				msg.data[j]=readIN[j];	
				msg.data.push_back(readIN[j]);
			}
			 chatter_pub.publish(msg);			

		}	
//	        chatter_pub.publish(msg);
                ros::spinOnce();

                loop_rate.sleep();
		k=0;
	//	memset(readIN,0,sizeof(readIN));
        }
	return 0;
} 
int inventory()
{
	 BlackLib::BlackSPI  mySpi(BlackLib::SPI1_0,8,BlackLib::SpiMode0,500000);


        bool isOpened = mySpi.open( BlackLib::ReadWrite | BlackLib::NonBlock);
        if( !isOpened )
        {
                ROS_INFO( "SPI DEVICE CAN\'T OPEN");
                ROS_INFO("value [%d]:",isOpened);
                exit(1);
        }
        else
        {
                 ROS_INFO( "SPI OPEN");
                 sleep(1);
        }
        uint8_t writeIN[6] = {0x00,0x04,0x03,0x26,0x01,0x00};/// UID
        uint8_t write0IN = 0x03;
        uint8_t write1IN[50] = {0x02};
       // uint8_t readIN[500];
       // uint8_t i;
        memset(readIN,0,sizeof(readIN));
        //memset(read1I,0,sizeof(read1I));
                mySpi.transfer(writeIN,readIN,sizeof(writeIN),5000);
                while (readIN[0] != 8)                                      //wait bit 3 ( EX: '0000 1000')
                {
                        readIN[0] = mySpi.transfer(write0IN,5000);
                        readIN[0] = readIN[0] & 0x08;//write poll 0x03
                        ROS_INFO("resceive data_Inventory= %02X\r\n",readIN[0]);
                        if(readIN[0] & 0x08)
                        {
                                ROS_INFO("exit Inventory \r\n");
                                break;
                        }
                }
                mySpi.transfer(write1IN,readIN,sizeof(write1IN),5000); //write command read data
                if(readIN[1] == 128)
                {


                        ROS_INFO("TAG DETECTED:  \r\n");
	//		memcpy(m,readIN,20);
                        ROS_INFO("UID: ");
			k=1;
                       // uint8_t len = readIN[2];
                       // for(i=1;i<len + 2;i++)
                       // {
                      //       ROS_INFO("%02X",readIN[i]);
			//	 readIN[i];
				// return readIN[i];
                        //}
		//	 return readIN[i];
                //        sleep(1);
		}
}
void protocol()
{
	 BlackLib::BlackSPI  mySpi(BlackLib::SPI1_0,8,BlackLib::SpiMode0,500000);

        bool isOpened = mySpi.open( BlackLib::ReadWrite | BlackLib::NonBlock);
        if( !isOpened )
        {

                //std::cout << "SPI DEVICE CAN\'T OPEN.;" << std::endl;
                //exit(1);
                ROS_INFO( "SPI DEVICE CAN\'T OPEN");
                ROS_INFO("value [%d]:",isOpened);
                exit(1);
        }
        else
        {
                 ROS_INFO( "SPI OPEN");
                sleep(1);
        }

        uint8_t writeP[5] = {0x00,0x02,0x02,0x01,0x05};
        uint8_t write0P =0x03;
        uint8_t write1P[3] = {0x02,0x00,0x00};
        uint8_t readP[200];
        memset(readP,0,sizeof(readP));
        mySpi.transfer(writeP,readP,sizeof(writeP),5000);
        while (readP[0] != 8)                                      //wait bit 3 ( EX: '0000 1000')
                {
                        readP[0] = mySpi.transfer(write0P,5000);
                        readP[0] = readP[0] & 0x08;//write poll 0x03
                        ROS_INFO("resceive data_protocol = %02X\r\n",readP[0]);
                        if(readP[0] & 0x08)
                        {
                                ROS_INFO("exit protocol \r\n");
                                break;
                        }

                }
        mySpi.transfer(write1P,readP,sizeof(write1P),5000); //write command read data
        ROS_INFO("resceive data_protocol[0] = %02X\r\n",readP[0]);//print data
        ROS_INFO("resceive data_protocol[1] = %02X\r\n",readP[1]);//print data
        ROS_INFO("resceive data_protocol[2] = %02X\r\n",readP[2]);//print data
        sleep(1);
        if(readP[1] == 0x00 && readP[2] == 0x00)
        {
                ROS_INFO("PROTOCOL SET \r\n");
                NFCReady = 1;//NFC is ready
                ROS_INFO("NFCReady=%d \r\n",NFCReady );

        }
        else
        {
                 ROS_INFO("RESPONSE TO SET PROTOCOL");
                 NFCReady = 0;// NOT NFC is ready
                 ROS_INFO("NFCReady=[%d] \r\n",NFCReady );
        }
        sleep(1);

}
