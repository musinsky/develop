{
  //------------------------------------------------------------  Create 7

  i2 = 7;
  i1 = 100;// - 1024;
  ii = 0;
  if(Cr_Exist[i2]) {

    while(fread(Data,1,4,fp[i2]) == 4  && (Data[3] != 0xB0)) {

      //if(ii%10 == 0) printf("\n");
      //printf(" %02X%02X%02X%02X",Data[3],Data[2],Data[1],Data[0]);
      //ii++;

      if((Data[3] & 0xF0) == 0x40) {
        hit   = ((Data[2] & 0xF8)>>3) + (Data[3] & 0x0F)*32;
        //delta = ((Data[1] & 0xF0)>>4) + (Data[2] & 0x07)*16;
        //ampl  =   Data[0]             + (Data[1] & 0x0F)*256;
        delta = Data[2] & 0x07;
        ampl  = Data[0] + Data[1]*256;
        if(i1 > 0 ) {
          TGepom::fgGemEvent->AddHit(i1 + hit, ampl, delta);

          //printf(" %d:%d:%d  ", hit+i1, ampl, delta);
        }
      }

      if(((Data[3] &  0xF0) == 0x80) && ((Data[2] & 0x0F) == 0x04)) i1 += 1024;
      if( Data[3] == 0xA0) iev7++;
    }
    //printf("\n");
    //printf(" %02X%02X%02X%02X\n",Data[3],Data[2],Data[1],Data[0]);
  }
}
