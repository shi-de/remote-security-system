#include "MFRC522.h"

#define 	MAXRLEN 18
#define   RC522_DELAY()  delay_us(2)


void MFRC522_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	/* ùùùù SPI_RC522_SPI ùùù?ùSDA */
	GPIO_InitStructure.GPIO_Pin = MFRC522_GPIO_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(MFRC522_GPIO_SDA_PORT, &GPIO_InitStructure);
	
	/* ùùùù SPI_RC522_SPI ùùù?ùSCK */
	GPIO_InitStructure.GPIO_Pin = MFRC522_GPIO_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(MFRC522_GPIO_SCK_PORT, &GPIO_InitStructure);
	
	/* ùùùù SPI_RC522_SPI ùùù?ùMOSI */
  GPIO_InitStructure.GPIO_Pin = MFRC522_GPIO_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(MFRC522_GPIO_MOSI_PORT, &GPIO_InitStructure);
	
	/* ùùùù SPI_RC522_SPI ùùù?ùMISO */
  GPIO_InitStructure.GPIO_Pin = MFRC522_GPIO_MISO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(MFRC522_GPIO_MISO_PORT, &GPIO_InitStructure);
	
	/* ùùùù SPI_RC522_SPI ùùù?ùRST */
  GPIO_InitStructure.GPIO_Pin = MFRC522_GPIO_RST_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(MFRC522_GPIO_RST_PORT, &GPIO_InitStructure);
	
}
	

/////////////////////////////////////////////////////////////////////
//ùùùùùùùùùùùùMFRC522ùù??ù?ùùùùù?ùùù?ùùùùù
//ùùùù?ùùùùAddress[IN]:ù?ùùùùù?
//ùù    ù?ùùùùùùù?
/////////////////////////////////////////////////////////////////////
unsigned char Read_MFRC522(unsigned char Address)
{
     unsigned char i, ucAddr;
     unsigned char ucResult=0;

     MFRC522_SCK_L;
		 RC522_DELAY();
     MFRC522_SDA_L;
		 RC522_DELAY();
     ucAddr = ((Address<<1)&0x7E)|0x80;

     for(i=8;i>0;i--)
     {
			 
         if(ucAddr&0x80)
				 {
						MFRC522_MOSI_H;
				 }
				 else
				 {
						MFRC522_MOSI_L;
				 }
				 RC522_DELAY();
         MFRC522_SCK_H;
				 RC522_DELAY();
         ucAddr <<= 1;
         MFRC522_SCK_L;
				 RC522_DELAY();
     }

     for(i=8;i>0;i--)
     {
         MFRC522_SCK_H;
			 RC522_DELAY();
         ucResult <<= 1;
         ucResult|= MFRC522_MISO_READ;
         MFRC522_SCK_L;
			 RC522_DELAY();
     }

     MFRC522_SCK_H;
		 RC522_DELAY();
     MFRC522_SDA_H;
		 RC522_DELAY();
     return ucResult;
}



/////////////////////////////////////////////////////////////////////
//ùùùùùùùùùùùùMFRC522ùù??ù?ùùù??ùùù?ùùùùù
//ùùùù?ùùùùAddress[IN]:ù?ùùùùù?
//          value[IN]:?ùùù?
/////////////////////////////////////////////////////////////////////
void Write_MFRC522(unsigned char Address, unsigned char value)
{  
    unsigned char i, ucAddr;

     MFRC522_SCK_L;
     MFRC522_SDA_L;
    ucAddr = ((Address<<1)&0x7E);

    for(i=8;i>0;i--)
    {
			   if(ucAddr&0x80)
				 {
						MFRC522_MOSI_H;
				 }
				 else
				 {
						MFRC522_MOSI_L;
				 }
				 RC522_DELAY();
         MFRC522_SCK_H;
				 RC522_DELAY();
         ucAddr <<= 1;
         MFRC522_SCK_L;
				 RC522_DELAY();
    }

    for(i=8;i>0;i--)
    {
			   if(value&0x80)
				 {
						MFRC522_MOSI_H;
				 }
				 else
				 {
						MFRC522_MOSI_L;
				 }
				 RC522_DELAY();
         MFRC522_SCK_H;
				 RC522_DELAY();
         value <<= 1;
         MFRC522_SCK_L;
				 RC522_DELAY();
    }

		 MFRC522_SCK_H;
		RC522_DELAY();
     MFRC522_SDA_H;
		RC522_DELAY();
}


/////////////////////////////////////////////////////////////////////
//ùù    ù?ùùù?RC522
//ùù    ùù: ù?ùùùùùMI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_Reset(void) 
{
	//unsigned char i;
    MFRC522_RST_H;
		delay_us (1);             
    MFRC522_RST_L;
		delay_us (1);                         
    MFRC522_RST_H;
		delay_us (1);        	

    //MFRC522_RST_H;
    Write_MFRC522(CommandReg,0x0F); //soft reset
    while(Read_MFRC522(CommandReg) & 0x10); //wait chip start ok

		delay_us (1);            

    
    Write_MFRC522(ModeReg,0x3D);            //ùùMifareùù??ùùCRCùù??0x6363
    Write_MFRC522(TReloadRegL,30);           
    Write_MFRC522(TReloadRegH,0);
    Write_MFRC522(TModeReg,0x8D);
    Write_MFRC522(TPrescalerReg,0x3E);
   Write_MFRC522(TxAutoReg,0x40);
    return MI_OK;
}

/////////////////////////////////////////////////////////////////////
//ùù    ù?ùùùRC522ù?ùùù?
//ùùùù?ùùùùreg[IN]:ù?ùùùùù?
//          mask[IN]:ùù??
/////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg,tmp | mask);  // set bit mask
}



/////////////////////////////////////////////////////////////////////
//ùù    ù?ùùùRC522ù?ùùù?
//ùùùù?ùùùùreg[IN]:ù?ùùùùù?
//          mask[IN]:ùù??
/////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp & ~mask);  // clear bit mask
} 


/////////////////////////////////////////////////////////////////////
//ùù    ù?ù?ùùRC522ùùISO14443ùù??
//ùùùù?ùùùùCommand[IN]:RC522ùùùùùù
//          pInData[IN]:?ùùRC522ùùù?ùùù?ùùùùùù
//          InLenByte[IN]:ùùùùùùù?ùù??ùùù
//          pOutData[OUT]:ùùù?ùù?ù?ùùùùùùùù
//          *pOutLenBit[OUT]:ùùùùùùù?ù?ùùùù
/////////////////////////////////////////////////////////////////////
char MFRC522_ToCard(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
       case PCD_AUTHENT:
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
       case PCD_TRANSCEIVE:
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
       default:
         break;
    }
   
    Write_MFRC522(ComIEnReg,irqEn|0x80);	//PCD_TRANSCEIVE?? ùùùùùù?ù
    ClearBitMask(ComIrqReg,0x80);			//IRQùù©ùùù
    Write_MFRC522(CommandReg,PCD_IDLE);  //?ùùùù?ùùùù
    SetBitMask(FIFOLevelReg,0x80);		//ùùùFIFO Flash ùùErrReg  BufferOvflùù?
    
    for (i=0; i<InLenByte; i++)
    {   
				Write_MFRC522(FIFODataReg, pInData[i]);    //ùùùùù??FIFO
		}
    Write_MFRC522(CommandReg, Command);   //ùùùùFIFOùùùù
   
    
    if (Command == PCD_TRANSCEIVE)
    {    
				SetBitMask(BitFramingReg,0x80);  //ùùùù??ùù?ùù
		}
    
		n = Read_MFRC522(ComIrqReg);
    i = 1500;//ùùùù?ùù?ù?ùùùùùùùùùM1ùùùùù?ù?ùù25ms
    do 
    {
         n = Read_MFRC522(ComIrqReg);
			
         i--;
			
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);
	      
    if (i!=0)
    {    
         if(!(Read_MFRC522(ErrorReg)&0x1B))
         {
             status = MI_OK;
             if (n & irqEn & 0x01)
             {   status = MI_NOTAGERR;   }
             if (Command == PCD_TRANSCEIVE)
             {
               	n = Read_MFRC522(FIFOLevelReg);
              	lastBits = Read_MFRC522(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = Read_MFRC522(FIFODataReg);    }
            }
         }
         else
         {   
					status = MI_ERR;   
				 }
        
   }
   

   SetBitMask(ControlReg,0x80);           // stop timer now
   Write_MFRC522(CommandReg,PCD_IDLE); 
   return status;
}



//ùùùùùùùù  
//ˇùùùùùùùù?ùùùù?ùùù?ùù?ùùùùùù1msù?ùù
void MFRC522_AntennaOn(void)
{
    unsigned char i;
    i = Read_MFRC522(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}


//ù?ùùùùù
void MFRC522_AntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}


//ùùùùùùùùùùùùMF522ùùùùCRC
//ùùùùùùùùùpIndata--?ùùùùCRCùùùùù?ùlen--ùùù?ùù?ùpOutData--ùùùùùCRCùùù
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    ClearBitMask(DivIrqReg,0x04);
    Write_MFRC522(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   Write_MFRC522(FIFODataReg, *(pIndata+i));   }
    Write_MFRC522(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = Read_MFRC522(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = Read_MFRC522(CRCResultRegL);
    pOutData[1] = Read_MFRC522(CRCResultRegM);
}


/////////////////////////////////////////////////////////////////////
//ùù    ù?ùùùù??ùùùùùùùù??
//ùù    ùù: ù?ùùùùùMI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_Halt(void)
{
    
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
		char status;
    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return MI_OK;
}




/////////////////////////////////////////////////////////////////////
//ùù    ù?ù?ùù
//ùùùù?ùù: req_code[IN]:?ùùùù?
//                0x52 = ?ùù?ùùùùùùù?ùùù14443Aùù?ù?ù
//                0x26 = ??ùùùùùùùù??ù?ù
//          pTagType[OUT]ùùùù?ùùù?ùùù
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//ùù    ùù: ù?ùùùùùMI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_Request(unsigned char req_code,unsigned char *pTagType)
{
   char status;  
   unsigned int  unLen;
   unsigned char ucComMF522Buf[MAXRLEN]; 

   ClearBitMask(Status2Reg,0x08);
   Write_MFRC522(BitFramingReg,0x07);
   SetBitMask(TxControlReg,0x03);
 
   ucComMF522Buf[0] = req_code;

   status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
   
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;  

	 }
   
   return status;
}



//ùù    ù?ùùùùù?ùù?ùù??ù?ù?ù?ùùùù?ù
//ùùùù?ùù: pSnr[OUT]:ùù?ùùù??ù4ù?ù
//ùù    ùù: ù?ùùùùùMI_OK 
char MFRC522_Anticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    Write_MFRC522(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];

         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}


/////////////////////////////////////////////////////////////////////
//ùù    ù?ù?ùùùù?
//ùùùù?ùù: pSnr[IN]:ùù?ùùù??ù4ù?ù
//ùù    ùù: ù?ùùùùùMI_OK
/////////////////////////////////////////////////////////////////////
char MFRC522_SelectTag(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}



/////////////////////////////////////////////////////////////////////
//ùù    ù?ùùù?ùù?ùùùù
//ùùùù?ùù: auth_mode[IN]: ùùùùùù???
//                 0x60 = ùù?Aùù?
//                 0x61 = ùù?Bùù? 
//          addr[IN]ùùùùù?
//          pKey[IN]ùùùùùù
//          pSnr[IN]ùùùù?ùùù??ù4ù?ù
//ùù    ùù: ù?ùùùùùMI_OK
/////////////////////////////////////////////////////////////////////               
char MFRC522_AuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = MFRC522_ToCard(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(Read_MFRC522(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}


/////////////////////////////////////////////////////////////////////
//ùù    ù?ùùù?M1ùù?ùùùùùù
//ùùùù?ùù: addr[IN]ùùùùù?
//          pData[OUT]ùùùùùùùùùùù?ù16ù?ù
//ùù    ùù: ù?ùùùùùMI_OK
///////////////////////////////////////////////////////////////////// 
char MFRC522_Read(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}


/////////////////////////////////////////////////////////////////////
//ùù    ù?ù?ùùù?ùM1ùù?ùù
//ùùùù?ùù: addr[IN]ùùùùù?
//          pData[IN]ùù?ùùùùùù?ù16ù?ù
//ùù    ùù: ù?ùùùùùMI_OK
/////////////////////////////////////////////////////////////////////                  
char MFRC522_Write(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = MFRC522_ToCard(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}


