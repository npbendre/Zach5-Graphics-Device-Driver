#define VMODE 0
#define BIND_DMA 1
#define START_DMA 7
#define V_OFF 0
#define V_800x600_75Hz 1
#define RENDER_TRAPEZOID_SPC (0x00100c00)

#define FIFOSpaceFree (0x0160)
#define R0 (0x0088)
#define G0 (0x0090)
#define B0 (0x0098)
#define dRdx (0x00e0)
#define dGdx (0x0e8)
#define dBdx (0x00f0)
#define dRdyDom (0x0030)
#define dGdyDom (0x0038)
#define dBdyDom (0x0040)
#define RasterizerCfg (0x0078)
#define XDom0 (0x0080)
#define dXDom (0x00c8)
#define XSub0 (0x00d0)
#define dXSub (0x00d8)
#define Y0 (0x00a0)
#define dY (0x00a8)
#define ZStartU (0x0048)
#define ZStartL (0x0050)
#define dZdyDomU (0x0058)
#define dZdyDomL (0x0060)
#define dZdxU (0x0068)
#define dZdxL (0x0070)
#define Count (0x00b0)
#define Render (0x00b8)
#define ContinueNewSub (0x00c0)

//registers to save 
#define Aperture1 (0x0150)
#define Aperture2 (0x0158)
#define FrameBufferWriteMask (0x0208)
#define DisconDFIFO (0x0200)
#define DisconFIFO (0x01d8)
#define BootAddress (0x01f0)
#define MemoryCfg (0x0118)
#define CtrlGraphics (0x0010)
#define HgEnd (0x0138)
#define ScreenBase (0x01c0)
#define VClkCtl (0x01c8)
#define ScreenStride (0x0178)
#define HTotal (0x0120)
#define HbEnd (0x0140)
#define HsStart (0x0128)
#define HsEnd (0x0130)
#define VTotal (0x0148)
#define VbEnd (0x01a0)
#define VsStart (0x0190)
#define VsEnd (0x0198)
#define CfgGraphics (0x0018)

//Reboot graphics card
#define Reboot (0x0000)

//resolution dependen registers:

#define HgEnd (0x0138)

//Control registers
#define InterruptLine (0x0188)
#define DisplayData (0x0180)
#define FIFOCtrl (0x01a8)
#define LineCount (0x01b0)
#define ScreenBaseRight (0x01b8)
#define FlagsInt (0x0110)
#define DMACtrl (0x01d0)
#define MemoryCtrl (0x01e0)
#define WriteMaskBypass (0x01e8)
#define BootAddress (0x01f0)
#define ChipConfig (0x0018)

//DMA registers
#define DMAAddress (0x00f8)
#define DMACount (0x0100)
#define IntFlags (0x0110)
#define IntEnable (0x0108)
