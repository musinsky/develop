#define NCRATES 8
#define NSCALERS 12
#define NPOMMES 6

#include <stdlib.h>
#include <time.h>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TSQLResult.h>
#include <TSQLRow.h>
#include <TH1.h>
#include <TDirectory.h>
#include <TCanvas.h>
#include <TPaveLabel.h>
#include <TApplication.h>

#include <TGepom.h>
#include <TMyEvent.h>

static TTree *dp = 0; 
static TTree *db = 0;

static Int_t  gThresh  = -2;
static Int_t  gDecod   = 0;

static Text_t gPath[] = "/home/alfa";
static Int_t  gBurst = 0;
static FILE *status;

//-----------------------------------------------------------------------
void RHistFill(Int_t Iburst)
{
  Int_t rq;
  Text_t t1[80], t2[80];
  FILE *fp;
  TH1F *h;

  TSQLResult *res;
  TSQLRow *row;

  Text_t sql[256];
  static const char *sel = "SELECT Data,Expr FROM histograms WHERE Numb ='%d'";

  gROOT->cd();   // !! I forget what it is -

  for (Int_t rq0 = 0; rq0<4; rq0++) {
 	   
    sprintf(t1,"%s/rq%1d",gPath,rq0); 
    fp = fopen(t1,"r");
    if (fp >0) { fscanf(fp,"%d",&rq); fclose(fp);}
    else printf("Sorry, cannot open %s\n",t1);

    sprintf(sql, sel, rq%100); 
    //printf("%s\n",sql);

    res = gGepom->Server()->Query(sql);

    row = res->Next();
    if(row) {
      sprintf(t2,"hist%02d",rq%100);
      h = (TH1F*)gDirectory->Get(t2);
      
      if(h > 0) {
	sprintf(t1,"%s %s %s",row->GetField(0),(rq<100)?">>":">>+",t2);
	dp->Draw(t1, row->GetField(1), "goff");

	if(rq < 100 && dp->GetEntries() == 0) h->Reset();

	sprintf(t1,"%s/h%1d",gPath,rq0);
	fp = fopen(t1,"w");
	if (fp > 0) {
	  Int_t  Nbin = h->GetNbinsX();
	  fprintf(fp,"%s\r\n %s\r\n %s\r\n %d\r\n %d\r\n %d\r\n"
		  ,h->GetTitle()
		  ,h->GetXaxis()->GetTitle()
		  ,h->GetYaxis()->GetTitle()
		  ,(Int_t)h->GetXaxis()->GetXmin()
		  ,(Int_t)h->GetXaxis()->GetXmax()
		  ,Nbin);
	  for (Int_t bin = 0; bin<Nbin+2; bin++)  
	    fprintf(fp,"%d\r\n",Int_t(h->GetBinContent(bin)));
	  fprintf(fp,"%d\r\n",Iburst);
	  fclose(fp);
	}
	else printf("Sorry, cannot open %s\n",t1);
      }      
      delete row;
    }
    delete res;
  }
}

//-----------------------------------------------------------------------
Int_t GetBurst(Int_t iburst)
{
  static Int_t  NbytesEv[NCRATES];
  static Int_t  NbytesBg[NCRATES];
  static Int_t  NbytesEd[NCRATES];
  static Bool_t Cr_Exist[NCRATES];

  static FILE *fp[NCRATES];

  static Int_t Init = 1;

  Int_t i0;

  if(Init) {
    Init = 0;

    for( i0 = 0; i0<NCRATES; i0++)  {
      Cr_Exist[i0] = kFALSE;
      if(i0 == 7) Cr_Exist[i0] = kTRUE;
      NbytesEv[i0] = 0;
      NbytesBg[i0] = 0;
      NbytesEd[i0] = 0;
    }

    FILE *fp0 = fopen(Form("%s/run%03d/naf", gPath, gGepom->CurrentRun()),"r");
    if (fp0 > 0) {
      Int_t cr,fc,nb;
      Char_t t;

      while(fscanf(fp0,"%d,%*d,%*d,%d,%d,%*d,%c.",&cr,&fc,&nb,&t) == 4) {
 
	printf("%d %d %d %c\n",cr,fc,nb,t);
 
	if(fc < 8) {
	  if(nb < 4) {
	    if(t == 'E') NbytesEv[cr] += nb;
	    if(t == 'N') NbytesBg[cr] += nb;
	    if(t == 'K') NbytesEd[cr] += nb;
	  }
	  Cr_Exist[cr] = kTRUE;
	}
      }
    fclose(fp0);}
  else printf("Sorry, cannot open naf file\n");
}
Text_t tt[80];

  UShort_t  Head_Data[4], Chck_Data1[1];
  UInt_t    Chck_Data[1];       
  UChar_t   Data[3000];

  Int_t i1, i2, ii, ret = 0, iev7 = 0;
  Int_t Nburst, Nevents, ievents = 0;

  for ( i2 = 0; i2<NCRATES; i2++) {
    if(i2 == 7) continue;
    sprintf(tt,"%s/run%03d/%07d", gPath, gGepom->CurrentRun(), i2*1000000+iburst);    
 
    fp[i2] = fopen(tt,"r");
    if (!fp[i2] && Cr_Exist[i2]) 
      {printf("%s for %s\n",gSystem->GetErrorStr(),tt); ret = 1; goto error;}

    if(Cr_Exist[i2]) fread(Head_Data,2,4,fp[i2]);
      
    Nburst  = Head_Data[1]*256*256 + Head_Data[0];
    Nevents = Head_Data[2];
    
    if(i2 == 0) ievents = Nevents;
    if( Nburst  != iburst )  {printf("Error 2.%1d\n",i2); ret = 2; goto error;}
    if( Nevents != ievents ) {printf("Error 6.%1d\n",i2); ret = 6; goto error;}

    //    if(i2 == 4) if(Cr_Exist[i2]) fread(Data,12,1,fp[i2]);
    if(i2 == 1 || i2 == 2 || i2 == 5 || i2 == 7) if(Cr_Exist[i2]) fread(Data,4,1,fp[i2]);
  }

  // fp[7] = fopen("/daq/nis/nis_2006-10-15_23-12-17_MSD.dat","r");
  fp[7] = fopen("/VME/vme.dat","r");

  UChar_t  work1;
  UShort_t width, wire, wire0, nb, ip;
  UShort_t ampl, hit, delta, nbr, ampl0;
  Int_t mt;
  long fpos;
  //  LInt_t work, work0;

  TGepom::fgHead[0]  = iburst; 

  for ( i0 = 0; i0<Nevents; i0++) { 
    //    printf("%d\n",i0);   
    if(!TGepom::fgPommeEvent) {printf("Error 7.%5d\n",i0); ret = 7; goto error;} 
    if(!TGepom::fgStrawEvent) {printf("Error 8.%5d\n",i0); ret = 8; goto error;} 
    if(!TGepom::fgGemEvent) {printf("Error 9.%5d\n",i0); ret = 8; goto error;} 

    TGepom::fgPommeEvent->Clear();
    TGepom::fgStrawEvent->Clear();
    TGepom::fgGemEvent->Clear();

    TGepom::fgPommeEvent->SetEventNumber(i0);
    TGepom::fgStrawEvent->SetEventNumber(i0);
    TGepom::fgGemEvent->SetEventNumber(i0);

    //------------------------------------------------------------  Create 0
    i2 = 0;
    if(NbytesEv[i2]) {
      if(fread(Data,NbytesEv[i2],1,fp[i2]) != 1) { 
	printf("***%d at fp %ld\n",i0, ftell(fp[i2]));
	//	printf("%d\n",i0);
	//	break; 
      }

      TGepom::fgHead[1]  = Data[0]; 
      TGepom::fgHead[2]  = i0;

      ip  = 2;
      for ( i1 = 0; i1<8; i1++) 
	TGepom::fgPs[i1]  = Data[2*i1+1+ip]*256 + Data[2*i1+0+ip];
      // TGepom::fgPs[i1]  = Data[3*i1+2+ip]*256*256 
      //	           + Data[3*i1+1+ip]*256 
      //	           + Data[3*i1+0+ip]; 
    }
 
    //------------------------------------------------------------  Create 5
    i2 = 5;
    if(NbytesEv[i2]) {
      fread(Data,NbytesEv[i2],1,fp[i2]); 
  
      for ( i1 = 0;i1<2;i1++) {
	TGepom::fgPs[i1 + 8] =  Data[4*i1+3]*256*256*256 + Data[4*i1+2]*256*256
	 + Data[4*i1+1]*256 + Data[4*i1+0];
	//	  printf("%x %x %d\n",Data[4*i1+i2],Data[4*i1+1+i2],gScalers[i1]);
      }      
    }
    //------------------------------------------------------------  Create 2
    /*
    i2 = 3;
    if(NbytesEv[i2]) {
      fread(Data,NbytesEv[i2],1,fp[i2]);                  

      ip = 0;
      for ( i1 = 0; i1<8; i1++) 
	TGepom::fgAdc[i1] = Data[2*i1+1+ip]*256 + Data[2*i1+ip];	   

      ip += 2*8;
      for ( i1 = 0; i1<8; i1++) 
	TGepom::fgTdc[i1] = Data[2*i1+1+ip]*256 + Data[2*i1+ip];
    }
    
    if(fread(Head_Data,2,1,fp[i2]) != 1 ) printf("Error ***%d\n",i2);

    nbr = 3*Head_Data[0]+Head_Data[0]%2;
    //printf("%d \n",nbr);
    if (nbr) {
    if(fread(Data,nbr,1,fp[i2]) != 1) printf("Error ***%d\n",i2); 
    } 
    
      i1 = 0;
      while (i1<Head_Data[0]) {
                    
	hit = Data[3*i1 + 2];
	ampl  = Data[3*i1 + 1]*256 + Data[3*i1];

	delta = 0;
	if(Data[3*i1 + 5] == hit - 1) {
	  delta = ampl - Data[3*i1 + 4]*256 - Data[3*i1 + 3]; // common stop
	  //delta = Data[3*i1 + 3] + Data[3*i1 + 4]*256 - ampl; // common start
	  i1++;
	}

	//printf("%d %d %d %d %d\n",i1,hit,ampl,delta,Head_Data[0]);
	TGepom::fgGemEvent->AddHit(hit, ampl, delta);
	i1++;
      }
    */
   
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

	if(((Data[3] &  0xF0) == 0x80) && (mt != 0x0A)) i1 += 512; 	//1024
	if( Data[3] == 0xA0) iev7++;		
      }
      //if(Data[3] == 0xB0) {printf("\n"); printf(" %02X%02X%02X%02X\n",Data[3],Data[2],Data[1],Data[0]); } 
    }

    /*      
//------------------------------------------------------------  Create 3  &  4
    if(NbytesEv[4]) {
      wire0 = 0;
      for ( i1 = 0; i1<NWIREPLANES; i1++) {  // old chambers
	if(i1 == 0) { fread(Data,NbytesEv[3],1,fp[3]); i2 = 0; }
	if(i1 == 2) { fread(Data,NbytesEv[4],1,fp[4]); i2 = 0; }
		
	width = 0;
	nb = gGepom->ChambBytes(i1) + i2;
 
	while ( i2 < nb ) {           

	  work1 = Data[i2++];

	  wire = wire0; 
	  while(work1) {
	    if(work1 & 0x1) width++;
	    else if(width) { TGepom::fgPommeEvent->AddWire(wire-width, width); width = 0; }
	    
	    wire++; work1 >>= 1;
	  }
	  wire0 += 8;
	  if(width && wire0 != wire) 
	    { TGepom::fgPommeEvent->AddWire(wire-width, width); width = 0; } // end byte
	}
	if(width) TGepom::fgPommeEvent->AddWire(wire-width, width);          // end chamber	
      }
    }
    */    
    /*
//------------------------------------------------------------  Create 1
    i2 = 1;
  
    if(fread(Head_Data,2,1,fp[i2]) != 1 ) printf("Error ***%d\n",i2);

    nbr = 3*Head_Data[0]+Head_Data[0]%2;
    //printf("%d \n",nbr);
    if (nbr) {
    if(fread(Data,nbr,1,fp[i2]) != 1) printf("Error ***%d\n",i2); 
    } 
    
      i1 = 0;
      while (i1<Head_Data[0]) {
                    
	hit = Data[3*i1 + 2];
	ampl  = Data[3*i1 + 1]*256 + Data[3*i1];
			
	delta = 0;
	if(Data[3*i1 + 5] == hit - 1) {
	  delta = ampl - Data[3*i1 + 4]*256 - Data[3*i1 + 3];   // common stop
	  //delta = Data[3*i1 + 3] + Data[3*i1 + 4]*256 - ampl;   // common start
	  i1++;
	}
	//TGepom::fgStrawEvent->AddStraw(hit, ampl, delta);
			
	//printf("%d %d %d %d %d\n",i1,hit,ampl,delta,Head_Data[0]);
	TGepom::fgGemEvent->AddHit(hit+64, ampl, delta);
	i1++;
      }
    */     
    /* 
      if(NbytesEv[i2]) fread(Data,NbytesEv[i2],1,fp[i2]);
      if(NbytesEv[i2]) {
      
      fread(Data,NbytesEv[i2],1,fp[i2]);                         // straws

      i1 = 0; 
      work = 0;
      while(Data[3*i1 + 2] != 0xFF && i1 < NbytesEv[i2]/3) {
      work0 = Data[3*i1 + 2]*256*256 + Data[3*i1 + 1]*256 + Data[3*i1];
      if(work == work0) break;
      //printf("%06lX ", work0);
      hit  = Data[3*i1 + 2];
      ampl  = Data[3*i1 + 1]*256 + Data[3*i1];
      delta = 0;
      if(Data[3*i1 + 5] == hit - 1) {
      // delta = adc - Evnt_Data[3*i1 + 4]*256 - Evnt_Data[3*i1 + 3];
      delta = Data[3*i1 + 3] + Data[3*i1 + 4]*256 - ampl;
      work0 = Data[3*i1 + 3] + Data[3*i1 + 4]*256 + Data[3*i1 + 5]*256*256;
      //printf("%06lX ", work0);
      i1++;
      }
      //else printf("       ");

      TGepom::fgStrawEvent->AddStraw(nadc, adc, delta);
      //if(!work) printf("\n");
      work = work0;
      i1++;
      }
    */     
    /*       
    ii = 0;
    if(fread(Head_Data,2,1,fp[i2]) != 1 ) printf("Error ***%d\n",i2);

    nbr = 3*Head_Data[0]+Head_Data[0]%2;
    if (nbr) {
 
    if(fread(Data,nbr,1,fp[i2]) != 1) printf("Error ***%d\n",i2); 
 
    if(Head_Data[0]>1024) {
      printf("\n***%d %d",i0,Head_Data[0]);
      for (i1 = 0; i1<50;i1++) {
      if(i1%10 == 0) printf("\n"); printf("%06lX ", Data[3*i1+2]*256*256 + Data[3*i1+1]*256 + Data[3*i1]);
      }
    }
    else {
      i1 = 0;
      while (i1<Head_Data[0]) {
                    
	hit = Data[3*i1 + 2];
	ampl  = Data[3*i1 + 1]*256 + Data[3*i1];
	delta = 0;
	if(Data[3*i1 + 5] == hit - 1) {
	  // delta = ampl - Evnt_Data[3*i1 + 4]*256 - Evnt_Data[3*i1 + 3];
	  delta = Data[3*i1 + 3] + Data[3*i1 + 4]*256 - ampl;
	  i1++;
	}
	//printf("%d %d %d %d %d\n",i1,hit,ampl,delta,Head_Data[0]);
	TGepom::fgStrawEvent->AddStraw(hit, ampl, delta);
	i1++;
      }
    }      
    */       
    /*          
      for ( i1 = 0; i1<NPOMMES; i1++) {                             // new chambers
	fread(Data,32,1,fp[4]);

	i2 = 0;
	while ( i2 < 16) {
	  ampl = Data[2*i2+1]*256 + Data[2*i2];

	  if(gThresh >= -1) { if(ampl > gThresh) TGepom::fgPommeEvent->AddHit(hit, ampl); }
	  else 
	    if(ampl > gGepom->HardTh(hit)) TGepom::fgPommeEvent->AddHit(hit, ampl);

	  i2++; hit++;
	}
      }
      */
    //}
    dp->Fill();
  }
  
  if(iev7 != ievents) {printf("Error in tdcs %d(VME) %d\n",iev7,ievents);}//  ret = 7; goto error;}  
       
  for ( i2 = 0; i2<NCRATES; i2++) if(Cr_Exist[i2]) 
    if(1
       && i2 != 1 
       && i2 != 2
       && i2 != 5 
       && i2 != 7
       ) 
      {
	Chck_Data[0] = 0;
	fread(Chck_Data,4,1,fp[i2]);
	if(Chck_Data[0] != 0x5A5AA5A5) {printf("Error 3.%1d %08X\n",i2,Chck_Data[0]); ret = 3; goto error;}
  }
  
  for ( i2 = 0; i2<NCRATES; i2++) if(NbytesBg[i2]) fread(Data,NbytesBg[i2],1,fp[i2]);

  for ( i2 = 0; i2<NCRATES; i2++) if(Cr_Exist[i2] && i2 != 7) 
    {
      Chck_Data[0] = 0;
      fread(Chck_Data,4,1,fp[i2]);
      if(Chck_Data[0] != ((i2 == 1 || i2 == 2 || i2 == 5)?0xAA55A5A5:0xAA555A5A)) 
      //if(Chck_Data[0] != 0xAA55A5A5) 
	{printf("Error 4.%1d %04X\n",i2,Chck_Data[0]); ret = 4; goto error;}
    }
  
  for ( i2 = 0; i2<NCRATES; i2++) if(NbytesEd[i2]) 
    {
      fread(Data,NbytesEd[i2],1,fp[i2]);
  
      /*
	   i2 = 0;
	   for ( i1 = 0;i1<2;i1++) {
	   gScalers[i1] =  Data[4*i1+3+i2]*256*256*256
	   + Data[4*i1+2+i2]*256*256
	   + Data[4*i1+1+i2]*256
	   + Data[4*i1+0+i2];
	   //	  printf("%x %x %d\n",Data[4*i1+i2],Data[4*i1+1+i2],gScalers[i1]);
	   } 
	   i2 = 4*6;
	   for ( i1 = 0; i1<5; i1++) {
	   gScalers[i1+6] = (Data[4*i1+3+i2]   >> 4)*10000000 
	   + (Data[4*i1+3+i2] & 0x0F)*1000000
	   + (Data[4*i1+2+i2]   >> 4)*100000 
	   + (Data[4*i1+2+i2] & 0x0F)*10000
	   + (Data[4*i1+1+i2]   >> 4)*1000 
	   + (Data[4*i1+1+i2] & 0x0F)*100
	   + (Data[4*i1+0+i2]   >> 4)*10 
	   + (Data[4*i1+0+i2] & 0x0F);
	   //	  printf("%x %x %d\n",Data[4*i1+i2],Data[4*i1+1+i2],gScalers[i1+6]);
	   }       
      */

      if(i2 == 0) {
	for ( i1 = 0; i1<NSCALERS; i1++) {
	  TGepom::fgScalers[i1] = Data[3*i1+2]*256*256 + Data[3*i1+1]*256 + Data[3*i1];
	}
      }
      if(i2 == 5) {
	   for ( i1 = 0;i1<2;i1++) {
	     TGepom::fgScalers[i1+NSCALERS] =  Data[4*i1+3]*256*256*256 + Data[4*i1+2]*256*256
	     + Data[4*i1+1]*256 + Data[4*i1+0];
	   //	  printf("%x %x %d\n",Data[4*i1+i2],Data[4*i1+1+i2],gScalers[i1]);
	   } 
      }

    }  
  db->Fill();
  
  UShort_t  Chck1;

  for ( i2 = 0; i2<NCRATES; i2++) if(Cr_Exist[i2]) 
   if(i2 != 1 && i2 != 2 && i2 != 5 && i2 != 7) 
      {
    Chck_Data1[0] = 5;
    Chck1 = 0xAA55;
    //    Chck1 = (i2 == 4)? 0x400: 0xAA55;
    fread(Chck_Data1,2,1,fp[i2]); 
    if(Chck_Data1[0] != Chck1) {printf("Error 5.%1d %04X\n",i2, Chck_Data1[0]); ret = 5; goto error;}
  }
  
 error:
  for ( i2 = 0; i2<NCRATES; i2++) 
    if(fp[i2] && Cr_Exist[i2]) fclose(fp[i2]);

  return ret;
}
 
//-----------------------------------------------------------------------
void RunPomme()
{
  static TCanvas *label;
  static TPaveLabel *pl1, *pl2;
  static Int_t  Events = 0;

  TSQLResult *res;
  TSQLRow *row;
  Int_t i, nrows, nev;

  if(!label) {                    // Init

  res = gGepom->Server()->Query("SELECT * FROM histograms");

  nrows = res->GetRowCount();

  for( i = 0; i<nrows; i++) {
    row = res->Next();
    new TH1F(Form("hist%02d",atoi(row->GetField(0))),row->GetField(1),atoi(row->GetField(2))
		  ,atof(row->GetField(3)),atof(row->GetField(4)));
    delete row;
  }
  delete res;

    res = gGepom->Server()->Query("SELECT d.Run,r.Bursts,r.Events 
               FROM runs AS r, data AS d WHERE r.Run = d.Run ORDER BY d.Timest_in DESC");

    row = res->Next();
    if(row) {
      gGepom->SetRun(atoi(row->GetField(0)));
      gBurst = atoi(row->GetField(1)); 
      Events = atoi(row->GetField(2)); 
      delete row;}
    delete res;

    label = new TCanvas("dial","pomme polarimeter",-670,0,300,80);
    pl1   = new TPaveLabel(0.,0.52,1.,1.,""); pl1->Draw(); 
    pl2   = new TPaveLabel(0.,0.,1.,0.48,""); pl2->Draw(); 
  }

  static Char_t PrevStatus = 'U';

  static const char *ins1 = "INSERT data VALUES ('%d','%d','%d','%s')";
  static const char *upd1 = "UPDATE runs SET Bursts='%d',Events='%d',Run_End ='%s',Time_End='%d' WHERE Run = '%d'";
  static const char *upd2 = "UPDATE runs SET Run_Date='%s',Run_Beg='%s',Time_Beg='%d' WHERE Run = '%d'";
  static const char *ins2 = "INSERT IGNORE runs SET Run='%d'";

  Int_t  Iburst, Irun;
  Char_t RunStatus;
  Char_t TimeSt[20];

  //----------------------------Get Status
  Int_t ii = fscanf(status,"%c .\\run%d\\ %d %s ", &RunStatus, &Irun, &Iburst, TimeSt);
  //  printf("%d\n",ii);
  if(ii != 4) return;

  Text_t t1[80], tt[80];
  Text_t sql[1024];

  Int_t tnow = time(NULL);
  time_t   t = tnow;

  if(PrevStatus  == 'E' && RunStatus == 'R') {  //----------------------------Status modified  

      sprintf(sql, ins2, Irun); 
      printf("%s\n",sql);

      delete gGepom->Server()->Query(sql);

      strftime(t1, 20, "%Y-%m-%d", localtime(&t));
      strftime(tt, 20, "%H:%M:%S", localtime(&t));
      sprintf(sql, upd2, t1, tt, tnow,Irun); 
      printf("%s\n",sql);

      delete gGepom->Server()->Query(sql);

      Events = 0;
    }

  PrevStatus = RunStatus;

  if(Irun != gGepom->CurrentRun()) {                  //-----------------------------New Run
    gBurst = 0;
    gGepom->SetRun(Irun);
  }

  if(RunStatus == 'R' && Iburst >= 0)              //----------------------------New Burst
    if(Iburst != gBurst) {

      Text_t dir[40];

      strftime(t1, 10, "%b-%d", localtime(&t));
      sprintf(dir, "%s/%s", gGepom->FilePath(), t1);
      gSystem->mkdir(dir);

      TFile *ff = TFile::Open(Form("%s/data-%d.root", dir, tnow),"recreate");

      dp = new TTree("pp","pomme tree(events)");
      //dp = new TTree("dp","pomme tree(events)");

      dp->Branch("head",&TGepom::fgHead,"nb/i:pol:ne");   
      dp->Branch("adc", &TGepom::fgAdc, "a1/s:a2:a3:a4:a5:a6:a7:a8:a9:a10:a11:a12");
      dp->Branch("tdc", &TGepom::fgTdc, "t1/s:t2:t3:t4:t5:t6:t7:t8:t9:t10:t11:t12");
      dp->Branch("ps",  &TGepom::fgPs,  "p1/s:p2:p3:p4:p5:p6:p7:p8:p9:p10"); 
      dp->BranchOld("event",  "TGemEvent",   &TGepom::fgGemEvent);
      //dp->BranchOld("straws", "TStrawEvent", &TGepom::fgStrawEvent);
      //dp->BranchOld("pommes", "TPommeEvent", &TGepom::fgPommeEvent);
 
      db = new TTree("bp","pomme tree(bursts)");
      //db = new TTree("db","pomme tree(bursts)");

      db->Branch("head",&TGepom::fgHead, "nb/i:pol");   
      db->Branch("burst",&TGepom::fgScalers,
		 "sc01/i:sc02:sc03:sc04:sc05:sc06:sc07:sc08:sc09:sc10:sc11:sc12:sc13:sc14");
;     
      if(Iburst < gBurst) gBurst = 0;
      for ( i = gBurst; i<Iburst; i++) if(!gDecod && GetBurst(i)) goto error; 

      nev = Int_t(dp->GetEntries());

      Events += nev;

      RHistFill(Iburst);

      ff->cd();
      dp->Write();
      db->Write();

      strftime(tt, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
      sprintf(sql, ins1, tnow, Irun, nev, tt); 
      printf("%s\n",sql);

      delete gGepom->Server()->Query(sql);

      strftime(t1, 20, "%H:%M:%S", localtime(&t));   
      sprintf(sql, upd1, Iburst, Events, t1, tnow, Irun); 
      printf("%s\n",sql);

      delete gGepom->Server()->Query(sql);

    error:
      ff->Close();
      gBurst = Iburst;
    }
                                                   // Set Label
  Color_t color;

  if     (RunStatus == 'R') color = kGreen;
  else if(RunStatus == 'P') color = kYellow;
  else if(RunStatus == 'E') color = kRed;     
  else                      color = kWhite;  
      
  pl1->SetFillColor(color);
  pl1->SetLabel(Form("%.8s  Run = %d  Burst = %d", TimeSt, Irun, Iburst));
  pl2->SetLabel(Form("Events = %d  Sum = %d", nev, Events));

  label->Modified();
  label->Update();
}  
 
//-----------------------------------------------------------------------
void RunPomme0(Int_t N)
{
  Int_t run = gGepom->CurrentRun();
  TFile *ff = TFile::Open(Form("run%03d.root", run),"recreate");

      dp = new TTree("pp","pomme tree(events)");
      //dp = new TTree("dp","pomme tree(events)");

      dp->Branch("head",&TGepom::fgHead,"nb/i:pol:ne");   
      dp->Branch("adc", &TGepom::fgAdc, "a1/s:a2:a3:a4:a5:a6:a7:a8:a9:a10:a11:a12");
      dp->Branch("tdc", &TGepom::fgTdc, "t1/s:t2:t3:t4:t5:t6:t7:t8:t9:t10:t11:t12");
      dp->Branch("ps",  &TGepom::fgPs,  "p1/s:p2:p3:p4:p5:p6:p7:p8:p9:p10"); 
      dp->BranchOld("event",  "TGemEvent",   &TGepom::fgGemEvent);
      //dp->BranchOld("straws", "TStrawEvent", &TGepom::fgStrawEvent);
      //dp->BranchOld("pommes", "TPommeEvent", &TGepom::fgPommeEvent);
 
      db = new TTree("bp","pomme tree(bursts)");
      //db = new TTree("db","pomme tree(bursts)");

      db->Branch("head",&TGepom::fgHead, "nb/i:pol");   
      db->Branch("burst",&TGepom::fgScalers,
		 "sc01/i:sc02:sc03:sc04:sc05:sc06:sc07:sc08:sc09:sc10:sc11:sc12:sc13:sc14");

      for (Int_t i = 0; i<N; i++) if(!gDecod && GetBurst(i)) printf("%d\n",i);// goto error; 

      ff->cd();
      dp->Write();
      db->Write();

    error:
      ff->Close();
}  

//-----------------------------------------------------------------------
int main(int argc, char **argv)
{
  if(argc > 1)  gDecod  = atoi(argv[1]);
  if(argc > 2)  gThresh = atoi(argv[2]);

   //  TROOT simple("pomme","Pomme");

  TApplication simple("Pomme", &argc, argv);

  new TGepom("mar11", "gem", "alp01", "qwertyasdf");
 
  // gGepom->SetRun(20); RunPomme0(2555); // make root file from original data
  
  Int_t st = open(Form("%s/status",gPath), O_RDONLY);
  status = fdopen(st,"r");
  
  while(1){
    gSystem->Sleep(1000);
    RunPomme();
    }   
           
  return 0;
}
