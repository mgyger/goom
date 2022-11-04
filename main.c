/*
Copyright 2014 Mark Owen
http://www.quinapalus.com
E-mail: goom@quinapalus.com

This file is free software: you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License
as published by the Free Software Foundation.

This file is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this file.  If not, see <http://www.gnu.org/licenses/> or
write to the Free Software Foundation, Inc., 51 Franklin Street,
Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "lpc13xx.h"

volatile int obuf[4][2]; // L,R samples being output
volatile int tbuf[4][2]; // L,R samples being prepared
volatile int i0cnt,i1cnt; // interrupt counters

#define NPOLY 16 // polyphony: must be a power of 2
#define NCHAN 16 // number of MIDI channels/patches: must be a power of 2

const short sintab[256]={ // sine table, linearly interpolated by oscillators:
// In Octave:
// a=round(sin(2*pi*([-63 -63:1:63 63]/252))*32767)
// reshape([a'(1:128) a'(2:129)-a'(1:128)]',1,256)

  -32767,0,-32767,10,-32757,31,-32726,51,-32675,71,-32604,91,-32513,112,-32401,132,-32269,152,-32117,172,
  -31945,191,-31754,212,-31542,231,-31311,250,-31061,270,-30791,289,-30502,308,-30194,327,-29867,345,-29522,364,
  -29158,381,-28777,400,-28377,417,-27960,435,-27525,452,-27073,468,-26605,485,-26120,502,-25618,517,-25101,533,
  -24568,548,-24020,563,-23457,578,-22879,592,-22287,606,-21681,619,-21062,632,-20430,645,-19785,657,-19128,670,
  -18458,680,-17778,692,-17086,703,-16383,712,-15671,722,-14949,732,-14217,740,-13477,749,-12728,757,-11971,764,
  -11207,771,-10436,778,-9658,783,-8875,790,-8085,794,-7291,798,-6493,803,-5690,806,-4884,810,-4074,811,
  -3263,814,-2449,816,-1633,816,-817,817,0,817,817,816,1633,816,2449,814,3263,811,4074,810,
  4884,806,5690,803,6493,798,7291,794,8085,790,8875,783,9658,778,10436,771,11207,764,11971,757,
  12728,749,13477,740,14217,732,14949,722,15671,712,16383,703,17086,692,17778,680,18458,670,19128,657,
  19785,645,20430,632,21062,619,21681,606,22287,592,22879,578,23457,563,24020,548,24568,533,25101,517,
  25618,502,26120,485,26605,468,27073,452,27525,435,27960,417,28377,400,28777,381,29158,364,29522,345,
  29867,327,30194,308,30502,289,30791,270,31061,250,31311,231,31542,212,31754,191,31945,172,32117,152,
  32269,132,32401,112,32513,91,32604,71,32675,51,32726,31,32757,10,32767,0,
  };

// product of the following two tables is exp_2 of 12-bit fraction in Q30
const unsigned short exptab0[64]={ // "top octave generator": round(2^15*(2.^([0:1:63]/64)))
  32768,33125,33486,33850,34219,34591,34968,35349,35734,36123,36516,36914,37316,37722,38133,38548,
  38968,39392,39821,40255,40693,41136,41584,42037,42495,42958,43425,43898,44376,44859,45348,45842,
  46341,46846,47356,47871,48393,48920,49452,49991,50535,51085,51642,52204,52773,53347,53928,54515,
  55109,55709,56316,56929,57549,58176,58809,59449,60097,60751,61413,62081,62757,63441,64132,64830};

const unsigned short exptab1[64]={ // fine tuning: round(2^15*(2.^([0:1:63]/64/64)))
  32768,32774,32779,32785,32790,32796,32801,32807,32812,32818,32823,32829,32835,32840,32846,32851,
  32857,32862,32868,32874,32879,32885,32890,32896,32901,32907,32912,32918,32924,32929,32935,32940,
  32946,32952,32957,32963,32968,32974,32979,32985,32991,32996,33002,33007,33013,33018,33024,33030,
  33035,33041,33046,33052,33058,33063,33069,33074,33080,33086,33091,33097,33102,33108,33114,33119};

unsigned char chup[NCHAN];     // channel controls updated?
unsigned char sus[NCHAN];      // sustain pedal position
unsigned short knob[24];       // raw 10-bit MSB-justified ADC results
unsigned char ctrl[NCHAN][24]; // 7-bit control values
short pbend[NCHAN];            // pitch bend position

struct egparams { // envelope generator parameters
  unsigned short a,d,s,r;
  };

struct egvars { // envelope generator variables
  unsigned short state;   // 0=off, 1=attacking, 2=holding, 3=decaying, 4=sustaining, 5=releasing
  unsigned short logout;  // logarithmic output 4Q12
  unsigned short out;     // linear output Q16
  };

// internal state in each voice
// chars then shorts then ints reduces average size of ldr:s with constant offsets
struct voicedata {
  unsigned char fk;                 // 0   filter constant
  signed char note;                 // 1   note number; b7: note is down
  signed char chan;                 // 2   channel to which this voice is allocated
  unsigned char vel;                // 3   note on velocity

  unsigned short o0p;              // 4     oscillator 0 waveshape constants
  unsigned short o0k0,o0k1;        // 6,8
  unsigned short o1p;              // 10    oscillator 1 waveshape constants
  unsigned short o1k0,o1k1;        // 12,14
  unsigned short o1egstate;        // 16    oscillator 1 envelope generator state
  unsigned short o1eglogout;       // 18    oscillator 1 envelope generator logarithmic output

  unsigned short vol;              // 20    current output level, copied from amplitude envelope generator output on zero-crossings
  unsigned short eg0trip;          // 22    count of update cycles since volume update
  struct egvars egv[2];            // 24    variables for amplitude and filter envelope generators

  unsigned short lvol,rvol;        // 36,38 multipliers for L and R outputs
  unsigned short o1egout;          // 40    oscillator 1 envelope generator linear output
  unsigned short o1vol;            // 42    current oscillator 1 level, copied from amplitude envelope generator output on zero-crossings

  int o0ph;                        // 44    oscillator 0 phase accumulator
  int o1ph;                        // 48    oscillator 1 phase accumulator
  int o0dph;                       // 52    oscillator 0 phase increment
  int o1dph;                       // 56    oscillator 1 phase increment

  int o1fb;                        // 60    oscillator 1 feedback value
  int lo,ba;                       // 64,68 filter state
  int out;                         // 72    last output sample from this voice

  int o1o;                         // 76    oscillator 1 output
  } vcs[NPOLY];

struct patchdata {
  unsigned short o1ega,o1egd;      // 0,2   oscillator 1 envelope generator parameters
  unsigned short o1vol;            // 4     oscillator 1 output level
  unsigned short lvol,rvol;        // 6,8   pan values
  unsigned char cut;               // 10    filter cutoff
  signed char fega;                // 11    filter sensitivity
  unsigned char res;               // 12    filter resonance
  unsigned char omode;             // 13    oscillator mode: 0=mix 1=FM 2=FM+FB
  struct egparams egp[2];          // 14,22 parameters for amplitude and filter envelope generators
  } patch[NCHAN];

// external assembler routines
extern void inton();  // interrupt enable
extern void intoff(); // interrupt disable
extern void wavupa(); // waveform generation code

// called twice per sample, once for L and once for R output ~70kHz=72MHz/1024
void CT16B0handler() {
  int i;
  unsigned int u;
  CT16B0_IR=1;    // clear interrupt
  i=i0cnt++&7;    // count interrupts
  if(i==0) {      // first of group of 8?
    (&GPIO1_MASKED_ACCESS)[0x100]=0x100;
    obuf[0][0]=tbuf[0][0];  // copy sample buffer
    obuf[0][1]=tbuf[0][1];
    obuf[1][0]=tbuf[1][0];
    obuf[1][1]=tbuf[1][1];
    obuf[2][0]=tbuf[2][0];
    obuf[2][1]=tbuf[2][1];
    obuf[3][0]=tbuf[3][0];
    obuf[3][1]=tbuf[3][1];
    (&GPIO1_MASKED_ACCESS)[0x100]=0;
    }
  u=obuf[i>>1][i&1]; // sample word
  SSP0_DR=u>>16;     // split into two 16-bit pieces
  SSP0_DR=u;
  }

// called once every 4 samples ~9kHz=72MHz/8192
void CT32B0handler() {
  int h,i,j,k,n;
  struct voicedata*v;
  struct patchdata*p;
  struct egparams*ep;
  struct egvars*ev;
  CT32B0_IR=1; // clear interrupt
  (&GPIO1_MASKED_ACCESS)[0x400]=0x400; // PIO1_10

  wavupa();
  (&GPIO1_MASKED_ACCESS)[0x400]=0; // PIO1_10
  (&GPIO1_MASKED_ACCESS)[0x400]=0x400; // PIO1_10

  h=i1cnt++;           // count interrupts
  v=vcs+(h&(NPOLY-1)); // choose a voice for eg processing
  p=patch+v->chan;     // find its parameters
  n=!!(h&NPOLY);       // choose an eg
  ep=p->egp+n;         // get pointers to parameters and variables
  ev=v->egv+n;
  i=ev->logout;
  j=(v->note&0x80)||sus[v->chan]!=0; // note down?
  if(ev->state==0) i=0;
  else {
    if(!j) ev->state=5; // exit sustain when note is released
    switch(ev->state) {
  case 1:
      i+=ep->a;  // attack
      if(i>=0x10000) i=0xffff, ev->state=2;
      break;
  case 2:
      i--;       // hold at top of attack
      if(i<=0xfff0) ev->state=3; // hold for 16 iterations
      break;
  case 3:
      i-=ep->d;  // decay
      if(i<ep->s) i=ep->s, ev->state=4;
      break;
  case 4:  // sustain
      break;
  case 5:
      i-=ep->r;  // release
      if(i<0) i=0, ev->state=0;
      break;
      }
    }
  ev->logout=i;
  if(i==0) ev->out=0;
  else ev->out=(exptab0[(i&0xfc0)>>6]*exptab1[i&0x3f])>>(31-(i>>12)); // compute linear output

  if(n==0) { // do oscillator 1 eg as well
    i=v->eg0trip;
    if(i>4) v->vol=ev->out;
    if(v->vol==ev->out) i=0;
    i++;
    v->eg0trip=i;
    i=v->o1eglogout;
    if(!j) v->o1egstate=1;
    if(v->o1egstate==0) { // attack
      i+=p->o1ega;
      if(i>=0x10000) i=0xffff, v->o1egstate=1;
    } else { // decay
      i-=p->o1egd;
      if(i<0) i=0;
      }
    v->o1eglogout=i;
    if(i==0) v->o1egout=0;
    else v->o1egout=(((exptab0[(i&0xfc0)>>6]*exptab1[i&0x3f])>>(31-(i>>12)))*p->o1vol)>>16; // compute linear output
  } else { // recalculate filter coefficient
    k=((p->cut*p->cut)>>8)+((v->egv[1].logout*((p->fega*v->vel)>>6))>>15);
    if(k<0) k=0;
    if(k>255) k=255;
    v->fk=k;
    }

  n=h&7; // determine A/D batch
  knob[n   ]=(short)ADC_DR0; // read three conversion results, one from each bank of 8 controls
  knob[n+ 8]=(short)ADC_DR1;
  knob[n+16]=(short)ADC_DR2;
  (&GPIO1_MASKED_ACCESS)[0x1c]=(n+1)<<2; // PIO1_2,3,4: switch muxes for next batch
  (&GPIO1_MASKED_ACCESS)[0x400]=0; // PIO1_10
  }

// derive frequency and volume settings from controller values for one voice
void setfreqvol(struct voicedata*v,unsigned char*ct) {
  int u,l;
  unsigned int f;
  int p,q,p0,p1,p2;

  struct patchdata*pa=patch+v->chan;

  // oscillator 0 frequency
  u=((v->note&0x7f)<<12)/12; // pitch of note, Q12 in octaves, middle C =5
  u+=pbend[v->chan]/12;      // gives +/- 2 semitones
  u-=287; // constant to give correct tuning for sample rate: log((72e6/2048)/(440*2^-0.75)/128)/log(2)*4096 for A=440Hz
  f=(exptab0[(u&0xfc0)>>6]*exptab1[u&0x3f])>>(10-(u>>12)); // convert to linear frequency
  v->o0dph=f;

  // oscillator 0 waveform
  l=f>>13; // compute slope limit: l=32768 at 1/8 Nyquist
  if(l>30000) l=30000;  // keep within sensible range
  if(l<1024) l=1024;

  // waveform has four periods p0-3: slope0, flat0, slope1, flat1
  p=0x8000-(ct[16]*248); // first half p=[1272,32768]
  q=0x10000-p;           // second half q=[32768,64264]
  p0=(p*(127-ct[17]))>>7;
  if(p0<l) p0=l; // limit waveform slope
  p1=p-p0;
  p2=(q*(127-ct[17]))>>7;
  if(p2<l) p2=l;
//  p3=q-p2; // not used
  v->o0p=(p0+p2)/2+p1; // constants used by assembler code
  v->o0k0=0x800000/p0;
  v->o0k1=0x800000/p2;

  // oscillator 1 frequency
  if(ct[7]>0x60) u=-0x1000-287;     // fixed "low" frequency
  else if(ct[7]>0x20) u=0x3000-287; // fixed "high" frequency
  u+=(ct[2]<<7)+(ct[3]<<3)-0x2200;
  f=(exptab0[(u&0xfc0)>>6]*exptab1[u&0x3f])>>(10-(u>>12));
  v->o1dph=f;

  // oscillator 1 waveform
  l=f>>13; // =32768 at 1/8 Nyquist
  if(l>30000) l=30000;
  if(l<1024) l=1024;

  p=0x8000-(ct[0]*248); // first half p=[1272,32768]
  q=0x10000-p;          // second half q=[32768,64264]
  p0=(p*(127-ct[1]))>>7;
  if(p0<l) p0=l;
  p1=p-p0;
  p2=(q*(127-ct[1]))>>7;
  if(p2<l) p2=l;
//  p3=q-p2; // not used
  v->o1p=(p0+p2)/2+p1;
  v->o1k0=0x800000/p0;
  v->o1k1=0x800000/p2;

  v->lvol=(pa->lvol*v->vel)>>7; // calculate output multipliers taking velocity into account
  v->rvol=(pa->rvol*v->vel)>>7;
  }

// process all controllers for given channel
void procctrl(int chan) {
  struct patchdata*pa=patch+chan;
  unsigned char*ct=ctrl[chan];
  int i;

  i=ct[6];
  if(i) i=(exptab0[(i&0xf)<<2])>>(7-(i>>4)); // convert oscillator 1 level to linear
  pa->o1vol=i;

  pa->o1ega=0xffff/(ct[4]*ct[4]/16+1);  // scale oscillator 1 eg parameters
  if(ct[5]==127) pa->o1egd=0;
  else           pa->o1egd=0xffff/(ct[5]*ct[5]+1);

  pa->egp[0].a=0xffff/(ct[12]*ct[12]/16+1);  // scale amplitude eg parameters
  pa->egp[0].d=0xffff/(ct[13]*ct[13]+1);
  if(ct[14]==0) pa->egp[0].s=0;
  else          pa->egp[0].s=0xc000+(ct[14]<<7);
  pa->egp[0].r=0xffff/(ct[15]*ct[15]+1);

  pa->egp[1].a=0xffff/(ct[8]*ct[8]/16+1);  // scale filter eg parameters
  pa->egp[1].d=0xffff/(ct[9]*ct[9]+1);
  i=ct[10]; // sustain level
  if(i) i=exptab0[(i&0xf)<<2]>>(7-(i>>4));
  pa->egp[1].s=i;
  pa->egp[1].r=0xffff/(ct[11]*ct[11]+1);

  pa->cut=ct[20]<<1;   // scale filter control parameters
  pa->fega=(ct[19]<<1)-128;
  pa->res=0xff-(ct[21]<<1);

  if(ct[18]<0x20) pa->omode=0;  // oscillator combine mode
  else if(ct[18]>0x60) pa->omode=2;
  else pa->omode=1;

  i=ct[22]; // volume
  if(i) i=exptab0[(i&0xf)<<2]>>(7-(i>>4)); // convert to linear
  pa->lvol=(sintab[254-(ct[23]&~1)]*i)>>15; // apply pan settings maintining constant total power
  pa->rvol=(sintab[128+(ct[23]&~1)]*i)>>15;
  for(i=0;i<NPOLY;i++) if(vcs[i].chan==chan) setfreqvol(vcs+i,ct); // update any affected voices
  }

void memset(volatile void*p,int c,int n) { for(;n>0;n--) *((unsigned char*)p++)=c; }


// system initialisation
void init() {
  intoff();
  i0cnt=i1cnt=0;

  GPIO1_DIR=0x51c; // set to output: PIO1_2,3,4,8,10

  SYSCON_PDRUNCFG=0xed00; // power up system oscillator and PLL (inter alia, including ADC)
  SYSCON_SYSPLLCTRL=0x25; // M=6, MSEL=00101 ; P=2, PSEL=01 ; 72MHz with 12MHz crystal
  SYSCON_SYSPLLCLKSEL=1; // select system oscillator as PLL input
  SYSCON_SYSPLLCLKUEN=0;
  SYSCON_SYSPLLCLKUEN=1; // effect change
  while((SYSCON_SYSPLLSTAT&1)==0) ; // wait for lock

  SYSCON_MAINCLKSEL=3; // select PLL output as main clock
  SYSCON_MAINCLKUEN=0;
  SYSCON_MAINCLKUEN=1; // effect change

  SYSCON_SYSAHBCLKCTRL=0x00012bdf; // clock everything except the UART

  IOCON_PIO1_7=0xc1; // TXD
  IOCON_PIO1_6=0xc1; // RXD

  SYSCON_SYSAHBCLKCTRL=0x00013bdf; // clock everything
  SYSCON_PRESETCTRL=0xffffffff; // remove reset from everything

  memset(obuf,0,sizeof(obuf)); // clear all buffers
  memset(tbuf,0,sizeof(tbuf));
  memset(sus,0,sizeof(sus));
  memset(pbend,0,sizeof(pbend));
  memset(vcs,0,sizeof(vcs));
  memset(ctrl,64,sizeof(ctrl));
  memset(chup,1,sizeof(chup));
  memset(patch,0,sizeof(patch));

  SYSCON_UARTCLKDIV=0x1;

  SYSCON_SSP0CLKDIV=0x8; // PCLK/8=9MHz clock for SSP
  IOCON_PIO0_8 =0xc2; // CT16B0_MAT0
  IOCON_PIO0_9 =0xc1; // MOSI0
  IOCON_PIO0_10=0xc2; // SCK0: SCKLOC default is correct
  IOCON_PIO0_11=0x02; // AD0
  IOCON_PIO1_0 =0x02; // AD1
  IOCON_PIO1_1 =0x02; // AD2
  IOCON_PIO1_2 =0xc1; // SA
  IOCON_PIO1_3 =0xc1; // SB
  IOCON_PIO1_4 =0xc0; // SC
  IOCON_PIO1_9 =0xc1; // CT16B1_MAT0

  ADC_CR=0x13f07; // channels 0-2, clock=1.125MHz, enough time for 3 rounds of conversions @ 9kHz

  SSP0_CR0=0x0000000f; // SPI, no inversions, 16-bit frames, divide by 2 for 4.5MHz bit clock
  SSP0_CPSR=2;
  SSP0_CR1=2; // enable SSP

  UART_FCR=0; // enable UART
  UART_FCR=1;
  UART_FCR=7;

  CT16B0_PR=0xf;   // PCLK/16 = bit rate
  CT16B0_MR0=63;   // 64 bit times per stereo output
  CT16B0_MCR=0x03; // reset on match + interrupt
  CT16B0_EMR=0x30; // toggle output on match: LRCIN

  CT16B1_PR=0x1;   // PCLK/2
  CT16B1_MR0=0x1;  // MCLK=PCLK/8
  CT16B1_MCR=0x02; // reset on match - 0x03 for interrupt as well
  CT16B1_EMR=0x30; // toggle output on match: MCLK

  CT32B0_PR=0xf;    // PCLK/16 = bit rate
  CT32B0_MR0=511;   // 512 bit times per 4 Sa output
  CT32B0_MCR=0x03;  // reset on match + interrupt
  CT32B0_TC=512-74; // first interrupt after 74 bit times puts CT16B0 interrupt in middle of idle time after CT32B0 interrupt completes

  NVIC_IPR10=0x80004000; // CT16B0 interrupt has priority over CT32B0 interrupt
  NVIC_ISER1=0xa00;      // enable CT16B0 and CT32B0 interrupt

  CT16B0_TCR=1; // start timers more or less together
  CT16B1_TCR=1;
  CT32B0_TCR=1;

  inton();
  }

// serial routines
void setbaud(int x) {
  UART_LCR=0x83; // 8N1, DLAB=1
  UART_DLL=x;
  UART_DLM=0;
  UART_LCR=0x03; // 8N1, DLAB=0
  }

// output one byte
void o1ch(char c) { while(!(UART_LSR&0x20)) {} UART_THR=c; }

char rxbuf[256];           // buffer for received bytes
unsigned char rxbp0,rxbp1; // indices into rxbuf

// wait for one byte, updating controller information in any spare time
char w1ch() {
  int c,i,j=0;

  for(;;) {
    unsigned short*kn=knob;
    unsigned char*ct=ctrl[0];
    for(i=0;i<24;i++) {
      while(UART_LSR&1) {
        c=UART_RBR;
        if(c<0xf8) rxbuf[rxbp1++]=c;   // empty receive FIFO into local buffer, skipping all realtime messages
        }
      if(rxbp0!=rxbp1) return rxbuf[rxbp0++];    // if there are any characters in our buffer, return immediately
      if(kn[i]<(ct[i]<<9)+ 0x40) ct[i]=kn[i]>>9; // update knob positions if any have moved by more than a small amount
      if(kn[i]>(ct[i]<<9)+0x380) ct[i]=kn[i]>>9;
      }
    chup[0]=1;                // flag channel 1 needs updating
    if(chup[j]) procctrl(j);  // update next channel
    chup[j]=0;                // reset update flag
    j=(j+1)&(NCHAN-1);        // next channel in sequence
    }
  }

// silence one voice
void stopv(struct voicedata*v) {
  intoff();
  v->egv[0].state=0; // stop aeg processing for voice
  v->egv[0].logout=0;
  v->egv[0].out=0;   // silence voice
  v->egv[1].state=0; // stop feg processing for voice
  v->egv[1].logout=0;
  v->egv[1].out=0;
  v->eg0trip=0;
  v->o1egstate=1;
  v->o1eglogout=0;
  v->o1egout=0;
  v->o1vol=0;
  v->o1o=0;
  v->o1fb=0;
  v->fk=0;
  v->chan=NCHAN-1;
  v->vol=0;
  v->out=0;
  v->o0ph=0x00000000;
  v->o0dph=0x00000000;
  v->o1dph=0x00000000;
  v->lo=v->ba=0;
  inton();
  }

int main() {
  int b0,b1,b2; // three bytes of message received
  int c,ch,i,mi,u,mu;
  struct voicedata*v;

  init();
  setbaud(144); // 31.25kbaud
  for(i=0;i<NPOLY;i++) stopv(vcs+i);
  while(UART_LSR&1) c=UART_RBR;
  rxbp0=0; rxbp1=0;
  b0=0x90; // start with running status = note on, channel 1
  goto l2;

l0:
  c=w1ch();
  if(c<128) goto l0; // wait for a status byte
l1:
  if(c==0xf0) goto sysex; // system exclusive?
  b0=c;                   // remember status byte
l2:
  c=w1ch();
  if(c>=128) goto l1; // new status byte received
  b1=c;
  c=w1ch();
  if(c>=128) goto l1; // new status byte received
  b2=c;
  // here we have a 3-byte message
  ch=b0&0x0f;        // channel number
  switch(b0&0xf0) {  // message type
case 0x80: // note off
    b2=0;  // set velocity to 0 and
           // fall through
case 0x90: // note on
    mu=-1; mi=0;
    for(i=0;i<NPOLY;i++) { // find a voice to use
      v=vcs+i;
      u=v->egv[0].state;
      if(u&&v->chan==ch&&(v->note&0x7f)==b1) break; // this channel+note combination already sounding? reuse the voice
      if(u==0) u=6; // preferentially steal an idle voice
      u=(u<<16)-v->egv[0].logout;
      if(u>mu) mu=u,mi=i; // find most advanced+quietest voice to steal
      }
    if(b2==0) { // note off
      if(i==NPOLY) goto l2; // ignore if note already gone
      vcs[i].note&=0x7f;    // allow exit from attack/sustain
      goto l2;
      }
    // here we have a note on message
    if(i<NPOLY) { // note already sounding?
      v=vcs+i;
      goto retrig;
      }
    // note on, not already sounding:
    v=vcs+mi; // point to voice to be stolen
    stopv(v); // silence it
retrig:
    intoff();
    v->note=b1|0x80;  // set up voice data
    v->chan=ch;
    v->vel=b2;
    inton();
    setfreqvol(v,ctrl[ch]);
    intoff();
    v->egv[0].state=1; // trigger note
    v->egv[1].state=1;
    v->eg0trip=0;
    v->o1egstate=0;
    v->o1eglogout=0;
    v->o1egout=0;
    v->out=0;
    inton();
    break;
case 0xb0:
    if(b1==120||b1>=123) { // all notes off
      for(i=0;i<NPOLY;i++) stopv(vcs+i);
      break;
      }
    if(b1==64) { // sustain pedal
      sus[ch]=b2;
      break;
      }
    if(b1>= 16&&b1< 32) { ctrl[ch][b1-16    ]=b2; chup[ch]=1; } // controller 0..15; set update flag
    if(b1>=102&&b1<110) { ctrl[ch][b1-102+16]=b2; chup[ch]=1; } // controller 16..23; set update flag
    break;
case 0xe0: // pitch bend
    pbend[ch]=(b2<<7)+b1-0x2000;
    for(i=0;i<NPOLY;i++) if(vcs[i].chan==ch) setfreqvol(vcs+i,ctrl[ch]); // update frequencies for all affected voices
    break;
default: goto l0; // not recognised: wait for a new status byte
    }
  goto l2; // maintain running status

sysex: // handle system exclusive messages
  c=w1ch();
  if(c>=128) goto l1; // new status byte received
  if(c!=0x7d) goto l0; // abort
  c=w1ch();
  if(c==1) { // dump knob positions
    o1ch(0xf0);
    o1ch(0x7d);
    o1ch(0x01);
    for(i=0;i<24;i++) o1ch(ctrl[0][i]);
    o1ch(0xf7);
    }
  goto l0; // wait for new status
  }

#ifndef __SSAT
inline int __SSAT(int x,unsigned n) { --n; return x>>n != x>>31 ? ((1<<n)-1) ^ x>>31 : x; }
#endif

// waveform generation code
void wavupa() {
#define QP 0x400000 // constant needed by oscillator kernel
// oscillator kernel
#define OKER(mod) ({ \
  int i,pos,sinvd; \
  unsigned t,mph; \
  ph+=dph;                      /* accumulate phase */ \
  mph=ph+(mod);                 /* add in frequency (actually phase) modulation */ \
  mph>>=16;                     /* rescale */ \
  pos=(per-mph)*k1;             /* compute position in sine given two possible slopes */ \
  if(pos>=+QP) pos=mph*k0;      /* determine which slope is required */ \
  if(pos<=-QP) pos=mph*k0-(k0<<16); \
  t=(unsigned short)pos>>10;    /* extract fractional part needed for interpolation */ \
  i=__SSAT(pos>>16,7);          /* clamp result to produce flat parts of waveform */ \
  sinvd=((int*)sintab+64)[i];   /* fetch sine value and difference */ \
  (short)sinvd+((sinvd*t)>>22); /* interpolated result */ \
  })

  int samp[4];                        // 4 samples on stack
  struct voicedata*v;
  memset(tbuf,0,sizeof(tbuf));        // clear output sample accumulator buffer
  for(v=vcs;v<vcs+NPOLY;v++) {        // points to the voice data
    struct patchdata*p=patch+v->chan; // get corresponding patch data for this voice
    int o1o;
    unsigned ph,dph,per,k0,k1,o1vol=v->o1vol;

    // fetch oscillator 1 parameters
    ph=v->o1ph,dph=v->o1dph,per=v->o1p,k0=v->o1k0,k1=v->o1k1;
    // switch on oscillator combine mode
    if(p->omode<2) { // omode=0,1 (mix, FM)
      // plain oscillator
      // compute 4 samples with output scaled by oscillator 1 level
      samp[0]=OKER(0)*o1vol;
      samp[1]=OKER(0)*o1vol;
      samp[2]=OKER(0)*o1vol;
      samp[3]=(o1o=OKER(0))*o1vol;
    } else { // omode=2 (FM+FB)
      // with FM feedback
      // compute 4 samples with output scaled by oscillator 1 level
      samp[0]=OKER(v->o1fb<<8)*o1vol; // get feedback value
      samp[1]=OKER(samp[0]<<8)*o1vol;
      samp[2]=OKER(samp[1]<<8)*o1vol;
      samp[3]=v->o1fb=(o1o=OKER(samp[2]<<8))*o1vol; // store feedback value for next time
      }
    v->o1ph=ph;
    if((v->o1o^o1o)<0) v->o1vol=v->o1egout; // update vol at zero-crossing of output
    v->o1o=o1o;

    // fetch oscillator 0 parameters
    ph=v->o0ph,dph=v->o0dph,per=v->o0p,k0=v->o0k0,k1=v->o0k1;
    if(p->omode<1) { // omode=0 (mix)
      // plain oscillator
      // generate 4 samples, summing results into output
      samp[0]=OKER(0)+(samp[0]>>14);
      samp[1]=OKER(0)+(samp[1]>>14);
      samp[2]=OKER(0)+(samp[2]>>14);
      samp[3]=OKER(0)+(samp[3]>>14);
    } else { // omode=1,2 (FM, FM+FB)
      // with FM
      // generate 4 samples
      samp[0]=OKER(samp[0]<<4);
      samp[1]=OKER(samp[1]<<4);
      samp[2]=OKER(samp[2]<<4);
      samp[3]=OKER(samp[3]<<4);
      }
    v->o0ph=ph;

// filter step using sample in x
// simple CSound-style second order filter
#define FILSTEP(x) \
  lo+=(((fk*ba)>>8)*fk)>>8, \
  ba+=(((((x)-((res*ba)>>8)-lo)*fk)>>8)*fk)>>8, \
  (x)=__SSAT(lo,17)    /* clamp output */

    // fetch filter parameters and state
    int fk=v->fk,res=p->res,lo=v->lo,ba=v->ba;
    FILSTEP(samp[0]);  // run filter over 4 samples
    FILSTEP(samp[1]);
    FILSTEP(samp[2]);
    FILSTEP(samp[3]);
    v->lo=lo,v->ba=ba; // save internal filter state

    unsigned amp[2];   // overall left and right multipliers
    int out=v->out;    // last output sample from this voice
    v->out=samp[3];
    unsigned vol;

// switch volume to aeg output
// scale by left and right amplitudes to get overall left and right multipliers
#define SWVOL0(sw) \
  if (sw) v->vol=v->egv[0].out; \
  vol=v->vol; \
  amp[0]=vol*v->lvol>>16; \
  amp[1]=vol*v->rvol>>16

    // immediate zero-crossing?
    if(out!=0&&(out^samp[0])<0) {
      SWVOL0(1);                  // switch volume
      goto waz0r;                 // drop into code below to process all four samples
      }
    // otherwise start at amplitude stored in vol, possibly switch later to aeg output if a zero-crossing is found
    SWVOL0(0);                    // update without switch
    // look for zero crossing within four samples
    if((samp[0]^samp[1])<0) {
      tbuf[0][0]+=amp[0]*samp[0]; // process one sample
      tbuf[0][1]+=amp[1]*samp[0];
      SWVOL0(1);                  // switch volume
      tbuf[1][0]+=amp[0]*samp[1]; // process one more sample
      tbuf[1][1]+=amp[1]*samp[1];
      goto waz2r;                 // drop into code below to process remaining two samples
      }
    if((samp[1]^samp[2])<0) {
      tbuf[0][0]+=amp[0]*samp[0]; // process two samples
      tbuf[0][1]+=amp[1]*samp[0];
      tbuf[1][0]+=amp[0]*samp[1];
      tbuf[1][1]+=amp[1]*samp[1];
      SWVOL0(1);                  // switch volume
      goto waz2r;                 // drop into code below to process remaining two samples
      }
    if((samp[2]^samp[3])<0) {
      tbuf[0][0]+=amp[0]*samp[0]; // process two samples
      tbuf[0][1]+=amp[1]*samp[0];
      tbuf[1][0]+=amp[0]*samp[1];
      tbuf[1][1]+=amp[1]*samp[1];
      tbuf[2][0]+=amp[0]*samp[2]; // process one sample
      tbuf[2][1]+=amp[1]*samp[2];
      SWVOL0(1);                  // switch volume
      tbuf[3][0]+=amp[0]*samp[3]; // process one more sample
      tbuf[3][1]+=amp[1]*samp[3];
      goto waz4r;                 // drop into code below to process next voice
      }

    // no zero-crossings within four samples
    // accumulate 4 scaled samples into output buffer
waz0r:
    tbuf[0][0]+=amp[0]*samp[0];   // accumulate 2 scaled samples into output buffer
    tbuf[0][1]+=amp[1]*samp[0];
    tbuf[1][0]+=amp[0]*samp[1];
    tbuf[1][1]+=amp[1]*samp[1];
waz2r:
    tbuf[2][0]+=amp[0]*samp[2];   // accumulate 2 scaled samples into output buffer
    tbuf[2][1]+=amp[1]*samp[2];
    tbuf[3][0]+=amp[0]*samp[3];
    tbuf[3][1]+=amp[1]*samp[3];
waz4r:
    ;
    }
  }
