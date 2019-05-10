{
  //------------------------------------------------------------  Create 7

  i2 = 7;

  i1  = 100 - 512; // all blocks
  //i1 -= 1024; // 1-t block
  //i1 -= 512;  // 2-d block
  ii = 0;
  ampl0 = 0;
  mt = 0;

  if(Cr_Exist[i2]) {


    while(fread(Data,1,4,fp[i2]) == 4  && (Data[3] != 0xB0)) {

      //if(ii%10 == 0) printf("\n");
      //printf(" %02X%02X%02X%02X",Data[3],Data[2],Data[1],Data[0]);
      //ii++;

      if((Data[3] &  0xF0) == 0x80) mt = (Data[2] & 0x0F);

      if((Data[3] & 0xF0) == 0x40) {
        hit   = ((Data[2] & 0xF8)>>3) + (Data[3] & 0x0F)*32;
        //delta = ((Data[1] & 0xF0)>>4) + (Data[2] & 0x07)*16;
        //ampl  =   Data[0]             + (Data[1] & 0x0F)*256;
        delta = Data[2] & 0x07;
        if(mt == 9 && (Data[3] & 0x0C) != 0) delta++;
        ampl  = Data[0] + Data[1]*256;
        if(i1 > 0) {
          if(mt != 9) TGepom::fgGemEvent->AddHit(i1 + hit, ampl - ampl0, delta);
          else {
            hit   = ((Data[2] & 0xF8)>>3);
            TGepom::fgGemEvent->AddHit(i1 + hit, ampl, delta);
          }
          if(ampl0 == 0) {
            if(hit != 260) printf("first word number not 360 %d\n",hit + i1);
            ampl0 = ampl - 6000;
          }
          delta = 2;
          //printf(" %d:%d:%d  ", hit+i1, ampl, delta);
        }
      }
      if((Data[3] & 0xF0) == 0x50 && mt == 9) {
        delta++;
        if((Data[3] & 0x0C) == 0) delta = 0;
        hit   = ((Data[2] & 0xF8)>>3);
        ampl  = Data[0] + (Data[1] & 0x3F)*256;
        if(i1 > 0) TGepom::fgGemEvent->AddHit(i1 + hit, ampl, delta);
      }

      if(((Data[3] &  0xF0) == 0x80) && (mt != 0x0A)) i1 += 512;  //1024
      if( Data[3] == 0xA0) iev7++;
    }
    //if(Data[3] == 0xB0) {printf("\n"); printf(" %02X%02X%02X%02X\n",Data[3],Data[2],Data[1],Data[0]); }
  }
}
