VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public Sub seodep2()

Dim head1 As String
Dim head2 As String

filout$ = "c:\tmp\out.txt"
Open filout$ For Output As #3

J = 790
170 PI = 3.14159265358979: E = 2.71828182845905
180 I = 200
190 Dim L$(18), LL$(18), TU$(4), DELT(200), q(200), ZZAT(200), QQ(790), VV(790), qd(790), vd(790), VPTP(200), AVP(200), LTP(365), QTP(365), H(202)
200 '********************** INPUT DATA FOR HEADING *****************************
210 CLS
head1 = "21"
head2 = ""
260 XMO = Val(Mid$(Date$, 1, 2))
320 '********************** SELECT BOUNDARIES **********************************
330 CLS
340 Print #3, "SELECT BOUNDARY CONDITIONS (ENTER ITEM NUMBER)"
350 Print #3, "    1) INFINITE AQUIFER"
360 Print #3, "    2) ALLUVIAL AQUIFER"
370 Print #3, "    3) NO FLOW BOUNDARY PERPENDICULAR TO STREAM"
380 Print #3, "    4) USE EFFECTIVE STREAM DEPLETION FACTOR TO APPROXIMATE EFFECT OF BOUNDARIES"
BI = 2
400 If BI = 4 Then ZZSEG$ = "N": GoTo 440
410 If BI > 4 Then GoTo 330
ZZSEG$ = "Y"
440 '********************* DRAW DIAGRAM ON SCREEN ******************************
450 For x = 1 To 18
460 L$(x) = " "
470 LL$(x) = " "
480 Print #3,
490 Next x
500 If ZZSEG$ = "N" Then GoTo 540
510 L$(1) = "      <--(-Z) : (+Z)-->"
520 L$(2) = "    <---Z1--->:<---Z2--->"
530 L$(3) = "    <------SEGMENT------>"
540 L$(4) = "============================="
550 L$(5) = "  STREAM"
560 L$(12) = "              O WELL"
570 If BI = 4 Then GoTo 890
580 If BI = 2 Then GoTo 760
590 For x = 6 To 11
600 L$(x) = "              :"
610 Next x
620 L$(5) = "  STREAM      :"
630 L$(8) = "              X"
640 If BI = 1 Then GoTo 890
650 L$(10) = "              :<-----B------>"
660 For x = 1 To 14
670 LL$(x) = "://///"
680 Next x
690 LL$(3) = "://N//"
700 LL$(4) = "://O//"
710 LL$(6) = "://F//"
720 LL$(7) = "://L//"
730 LL$(8) = "://O//"
740 LL$(9) = "://W//"
750 GoTo 890
760 For x = 6 To 11
770 L$(x) = "              :       :"
780 Next x
790 L$(5) = "  STREAM      :       :"
800 L$(8) = "              X       W"
810 L$(13) = "                      :"
820 L$(14) = "                      :"
830 For x = 16 To 18
840 L$(x) = "/////////////////////////////"
850 Next x
860 L$(15) = "-----------------------------"
870 L$(17) = "///////////NO FLOW///////////"
880 L$(12) = "              O WELL  :"
890 For x = 1 To 18
900 Print #3, L$(x); Tab(30); LL$(x)
910 Next x
920 Print #3,
930 Print #3,
940 '********************* INPUT AQUIFER AND BOUNDARY PARAMETERS **************
960 W = 8000
TR = 30000
S = 0.15
DXX = 3000
1010 If BI = 2 And (W - DXX) < 0 Then Print #3, "X CANNOT EXCEED W": GoTo 960
1020 If ZZSEG$ = "N" Then GoTo 1070
1030 Z1 = 0
1040 Z2 = 1000
1050 If BI = 3 And B < Z2 Then Print #3, "Z2 CANNOT EXCEED B ": GoTo 1040
1060 If (Z2 - Z1) < 0 Then Print #3, "Z1 CANNOT EXCEED Z2": GoTo 1030
1070 '************** DRAW DIAGRAM ON SCREEN WITH DIMENSIONS ********************
1080 Rem DRAW DIAGRAM ON SCREEN WITH DIMENSIONS
1090 For x = 1 To 10
1100 Print #3,
1110 Next x
1120 If ZZSEG$ = "N" Then GoTo 1300
1130 Print #3, "Z1="; Z1; "FEET,  Z2="; Z2; "FEET"
1140 If Z1 < 0 Then GoTo 1170
1150 Print #3, Tab(15); ":<--Z1-->"; Tab(30); LL$(1)
1160 GoTo 1180
1170 Print #3, "<------Z1---->:"; Tab(30); LL$(1)
1180 If Z2 < 0 Then GoTo 1240
1190 If Z2 = B Then GoTo 1220
1200 Print #3, Tab(15); ":<----Z2---->"; Tab(30); LL$(2)
1210 GoTo 1250
1220 Print #3, Tab(15); ":<-----Z2----->"; LL$(2)
1230 GoTo 1250
1240 Print #3, "        <-Z2->:"; Tab(30); LL$(2)
1250 Print #3, Tab(30); LL$(3)
1260 For x = 4 To 7
1270 Print #3, L$(x); Tab(30); LL$(x)
1280 Next x
1290 GoTo 1330
1300 For x = 1 To 7
1310 Print #3, L$(x); Tab(30); LL$(x)
1320 Next x
1330 If BI = 4 Then GoTo 1470
1340 If BI = 2 Then GoTo 1370
1350 Print #3, Tab(11); DXX; "'"; Tab(30); LL$(8)
1360 GoTo 1380
1370 Print #3, Tab(11); DXX; "'"; Tab(21); W; "'"; Tab(30); LL$(8)
1380 Print #3, L$(9); Tab(30); LL$(9)
1390 If BI = 3 Then GoTo 1420
1400 Print #3, L$(10); Tab(30); LL$(10)
1410 GoTo 1430
1420 Print #3, Tab(15); ":<-"; B; "'"; Tab(28); "->"; LL$(10)
1430 For x = 11 To 18
1440 Print #3, L$(x); Tab(30); LL$(x)
1450 Next x
1460 GoTo 1520
1470 For x = 8 To 12
1480 Print #3, L$(x)
1490 Next x
1500 Print #3, "SDF="; SDF; "DAYS"
1510 GoTo 1530
1520 Print #3, "T="; TR; "GPD/FT,  S="; S
1530 Print #3, "NOT DRAWN TO SCALE"
1830 Rem ***********************************************************************
1840 Rem INPUT PUMPING DATA
1850 CLS
1860 Print #3, "A PUMPING PERIOD IS A PERIOD OF TIME DURING WHICH THE PUMPING RATE IS CONSTANT"
1870 TT = 0
1880 Print #3,
CSI$ = "N"
1900 If CSI$ = "N" Or CSI$ = "n" Then GoTo 1950
NC = 1
NPPC = 10
1930 np = NC * NPPC
1940 GoTo 1970
1950 Print #3, "ENTER NUMBER OF PUMPING PERIODS"
np = 10
1970 Print #3, "ENTER NUMBER FOR TIME UNITS TO BE USED"
1980 Print #3, "   (1)DAYS"
1990 Print #3, "   (2)WEEKS - A WEEK WILL = 365/52 DAYS"
2000 Print #3, "   (3)MONTHS - A MONTH WILL = 365/12 DAYS"
2010 Print #3, "   (4)YEARS - A YEAR WILL = 365 DAYS"
TUI = 3
TU$(1) = "DAYS"
TU$(2) = "WEEKS"
TU$(3) = "MONTHS"
TU$(4) = "YEARS"
fact = 30.41667
2060 If CSI$ = "Y" Or CSI$ = "y" Then GoSub 6940: GoTo 2160
DELT(1) = 4
DELT(2) = 8
DELT(3) = 4
DELT(4) = 8
DELT(5) = 4
DELT(6) = 8
DELT(7) = 4
DELT(8) = 8
DELT(9) = 4
DELT(10) = 8
q(1) = 1000
q(2) = 0
q(3) = 1000
q(4) = 0
q(5) = 1000
q(6) = 0
q(7) = 1000
q(8) = 0
q(9) = 1000
q(10) = 0

2070 For x = 1 To np
2110 TT = TT + DELT(x)
2120 ZZAT(x) = TT
2150 Next x
2160 '******************* PRINT PUMPING SCHEDULE ON SCREEN *********************
2170 CLS
2180 LCOUNT = 5
2190 Print #3, Tab(15); "PUMPING SCHEDULE"
2200 Print #3, "TOTAL TIME SIMULATED = "; TT; TU$(TUI)
2210 Print #3,
2220 Print #3, "PUMPING", "Q", "LENGTH", "ACC. TIME"
2230 Print #3, "PERIOD", "GPM", TU$(TUI), TU$(TUI)
2240 For x = 1 To np
2250 Print #3, x, q(x), DELT(x), ZZAT(x)
2260 LCOUNT = LCOUNT + 1
2280 Next x
2330 '****************** SELECT TIME BETWEEN PRINTOUTS *************************
2340 CLS
2350 Print #3, "A LARGER NUMER OF TIME STEPS CAN BE SELECTED BY ANSWERING THE NEXT QUESTION     WITH A YES(Y)."
PFOT$ = "N"
2370 If PFOT$ = "N" Or PFOT$ = "n" Then GoTo 2420 Else PFOT$ = "Y"
2380 Print #3, "ENTER NUMBER OF  "; TU$(TUI); " BETWEEN PRINTOUTS."
TBP = 1
2400 If (TBP - Int(TBP)) > 0.0001 Then Print #3, "YOU MUST SELECT A WHOLE NUMBER OF "; TU$(TUI): GoTo 2380
2410 '********************* SELECT GRAPH OPTION ********************************
2420 GI$ = "N"
2430 If GI$ = "Y" Or GI$ = "y" Then GI$ = "Y" Else GI$ = "N"
2440 '************** PRINT HEADING, DIAGRAM AND PUMPING SCHEDULE ***************
2450 Print #3,
2460 Print #3,
2470 Print #3, "********************************************************************************"
2480 CLS:  Print #3, "THINKING, PLEASE WAIT"
2490 Print #3,
2500 Print #3, "                   STREAM DEPLETION USING GLOVER TECHNIQUES"
2510 Print #3,
2520 TA1 = (80 - Len(head1$)) / 2: TA2 = (80 - Len(head2$)) / 2: TA3 = (80 - Len(DTE$)) / 2
2530 Print #3, Tab(TA1); head1$: Print #3, Tab(TA2); head2$: Print #3, Tab(TA3); DTE$
2540 Print #3,
2550 Print #3, "********************************************************************************"
2560 Print #3,
2570 Print #3,
2580 If ZZSEG$ = "N" Then GoTo 2760
2590 Print #3, "Z1="; Z1; "FEET,  Z2="; Z2; "FEET"
2600 If Z1 < 0 Then GoTo 2630
2610 Print #3, Tab(15); ":<--Z1-->"; Tab(30); LL$(1)
2620 GoTo 2640
2630 Print #3, "<------Z1---->:"; Tab(30); LL$(1)
2640 If Z2 < 0 Then GoTo 2700
2650 If Z2 = B Then GoTo 2680
2660 Print #3, Tab(15); ":<----Z2---->"; Tab(30); LL$(2)
2670 GoTo 2710
2680 Print #3, Tab(15); ":<-----Z2----->"; LL$(2)
2690 GoTo 2710
2700 Print #3, "        <-Z2->:"; Tab(30); LL$(2)
2710 Print #3, Tab(30); LL$(3)
2720 For x = 4 To 7
2730 Print #3, L$(x); Tab(30); LL$(x)
2740 Next x
2750 GoTo 2790
2760 For x = 1 To 7
2770 Print #3, L$(x); Tab(30); LL$(x)
2780 Next x
2790 If BI = 4 Then GoTo 2930
2800 If BI = 2 Then GoTo 2830
2810 Print #3, Tab(11); DXX; "'"; Tab(30); LL$(8)
2820 GoTo 2840
2830 Print #3, Tab(11); DXX; "'"; Tab(21); W; "'"; Tab(30); LL$(8)
2840 Print #3, L$(9); Tab(30); LL$(9)
2850 If BI = 3 Then GoTo 2880
2860 Print #3, L$(10); Tab(30); LL$(10)
2870 GoTo 2890
2880 Print #3, Tab(15); ":<-"; B; "'"; Tab(28); "->"; LL$(10)
2890 For x = 11 To 18
2900 Print #3, L$(x); Tab(30); LL$(x)
2910 Next x
2920 GoTo 2980
2930 For x = 8 To 12
2940 Print #3, L$(x)
2950 Next x
2960 Print #3, "SDF="; SDF; "DAYS"
2970 GoTo 2990
2980 Print #3, "T="; TR; "GPD/FT,   S="; S
2990 Print #3, "NOT DRAWN TO SCALE"
3000 '**** GET LENGTH OF PUMP. PERIODS AND # OF TIME STEPS FOR CALC. PURPOSES **
3010 If PFOT$ = "Y" Then GoTo 3090
3020 MN = 1
3070 dela = MN
3080 GoTo 3100
3090 dela = TBP
3100 ADF = 0
3110 For x = 1 To np
3120 A = DELT(x) / dela
3130 If (A - Int(A)) = 0 Then GoTo 3150
3140 ADF = 1
3150 Next x
3160 If (TBP / dela - Int(TBP / dela)) > 0 Then GoTo 3180
3170 If ADF = 0 Then GoTo 3200
3180 dela = dela - 1
3190 GoTo 3100
3200 NPA = TT / dela
3210 '********** PRINT PUMPING SCHEDULE AND BRANCH TO SOLVE ROUTINES ***********
3220 GoSub 6790
3230 GoSub 6550
3240 Print #3, "COMPUTATIONS BEGIN AT "; Time$
3250 If ZZSEG$ = "Y" Or ZZSEG$ = "y" Then GoTo 4240
3260 If BI = 2 Then GoTo 3360
3270 '******** SOLVE DEPLETION FOR OPTION 1, 3 AND 4 (ENTIRE STREAM) ***********
3280 For x = 1 To NPA
3290 T = T + dela
3300 If BI = 4 Then U = (SDF / (4 * fact * T)) ^ 0.5 Else U = DXX / ((4 * TR * fact * T / (S * 7.481)) ^ 0.5)
3310 GoSub 5490
3320 QQ(x) = BQQ
3330 VV(x) = SVV
3340 Next x
3350 GoTo 3600
3360 '*************** SOLVE DEPLETION FOR OPTION 2 *****************************
3370 For x = 1 To NPA
3380 T = T + dela
3390 QS = 0
3400 VS = 0
3410 YY = -DXX
3420 FAC = 1
3430 YY = YY + 2 * DXX
3440 U = YY / ((4 * TR * fact * T / (S * 7.481)) ^ 0.5)
3450 GoSub 5490
3460 QS = QS + BQQ * FAC
3470 VS = VS + SVV * FAC
3480 If BQQ = 0 Then GoTo 3570
3490 YY = YY - 2 * DXX + 2 * W
3500 U = YY / ((4 * TR * fact * T / (S * 7.481)) ^ 0.5)
3510 GoSub 5490
3520 QS = QS + BQQ * FAC
3530 VS = VS + SVV * FAC
3540 If BQQ = 0 Then GoTo 3570
3550 FAC = FAC * (-1)
3560 GoTo 3430
3570 QQ(x) = QS
3580 VV(x) = VS
3590 Next x
3600 '******** USING SUPERPOSITION, GENERATE DEPLETION VALUES FOR PRINTING *****
3610 ADEL = 1
3620 For J = 1 To np
3630 N = 0
3640 If q(J) = 0 Then GoTo 3770
3650 For x = ADEL To NPA
3660 N = N + 1
3670 qd(x) = qd(x) + QQ(N) * q(J)
3680 vd(x) = vd(x) + VV(N) * q(J) * fact * N * dela
3690 Next x
3700 N = 0
3710 If np = 1 Then GoTo 3780
3720 For x = (1 + ZZAT(J) / dela) To NPA
3730 N = N + 1
3740 qd(x) = qd(x) - QQ(N) * q(J)
3750 vd(x) = vd(x) - VV(N) * q(J) * fact * N * dela
3760 Next x
3770 ADEL = 1 + ZZAT(J) / dela
3780 Next J
3790 '************************ PRINT DEPLETION *********************************
3800 Print #3, "COMPUTATIONS END AT "; Time$
3810 Print #3, "PLEASE WAIT FOR OUTPUT."
3820 Print #3,
GoSub 8020
3830 Print #3,
3840 Print #3,
3850 If np > 1 Then GoTo 3920
3860 Print #3, Tab(34); "STREAM DEPLETION"
3870 Print #3,
3880 Print #3, "                                                                         VOL. OF DEP."
3890 Print #3, "          TIME     DEP. RATE     DEP. RATE  VOL. OF DEP.  VOL. OF DEP.     THIS STEP"
3900 Print #3, ZZ$; "         (GPM)           (%)"; ZZZ$; "           (%)"; ZZZ$
3910 GoTo 3970
3920 Print #3, Tab(20); "STREAM DEPLETION"
3930 Print #3,
3940 Print #3, "                                            VOL. OF DEP."
3950 Print #3, "          TIME     DEP. RATE  VOL. OF DEP.     THIS STEP"
3960 Print #3, ZZ$; "         (GPM)"; ZZZ$; ZZZ$
3970 Print #3,
3980 If VUNIT$ = "GALLONS" Then GoTo 4010
3990 VFAC = 1440 / 325900!
4000 GoTo 4030
4010 VFAC = 1440
4020 VDTP = 0
4030 For x = 1 To NPA
4040 If PFOT$ = "Y" Then GoTo 4140
4050 PFLG = 0
4060 TDELA = TDELA + dela
4070 For J = 1 To np
4080 If TDELA = ZZAT(J) Then GoTo 4100
4090 GoTo 4110
4100 PFLG = 1
4110 Next J
4120 If PFLG = 1 Then GoTo 4160
4130 GoTo 4210
4140 If ((x * dela / TBP) - (Int(x * dela / TBP))) = 0 Then GoTo 4160
4150 GoTo 4210
Print #3, "np = "; np, "at line 4150"
4160 If np > 1 Then GoTo 4190
If opt5 = 2 Then GoSub 8200
4170 Print #3, USING; "############"; dela * x;: Print #3, Tab(15);: Print #3, USING; "#########.####"; qd(x); qd(x) * 100 / q(1); vd(x) * VFAC; vd(x) * 100 / (q(1) * dela * x * fact); (vd(x) * VFAC - VDTP)
4180 GoTo 4200
4190 Print #3, USING; "############"; dela * x;: Print #3, Tab(15);: Print #3, USING; "#########.####"; qd(x); vd(x) * VFAC; (vd(x) * VFAC - VDTP)
If opt5 = 2 Then GoSub 8200
4200 VDTP = vd(x) * VFAC
4210 Next x
4220 If GI$ = "Y" Then GoTo 5650
If opt5 = 2 Then Close #3
4230 End
4240 '*************** SOLVE DEPLETION FOR SEGMENT OF STREAM ********************
4250 If Z1 = -99999! Then IFLG = 1: Z1 = 0
4260 If Z2 = 99999! Then IFLG = 1: Z2 = 0
4270 YY = DXX
4280 QL = 0
4290 QN = 0
4300 On BI GoTo 4310, 4540, 5040
4310 L1 = Z1
4320 L2 = Z2
4330 For x = 1 To NPA
4340 T = T + dela
4350 U = YY / ((4 * TR * fact * T / (S * 7.481)) ^ 0.5)
4360 If U > 2.9 Then GoTo 4510
4370 If IFLG = 0 Then GoTo 4410
4380 GoSub 5490
4390 QQ(x) = 0.5 * BQQ
4400 VV(x) = 0.5 * SVV
4410 U = 4 * TR * T * fact / (S * 7.481)
4420 GoSub 7130
4430 QQ(x) = QQ(x) + BQQ
4440 QN = BQQ
4450 U = 4 * TR * (T - dela / 2) * fact / (S * 7.481)
4460 GoSub 7130
4470 QP = BQQ
4480 GoSub 7300
4490 AVV = AVV + SVV
4500 VV(x) = VV(x) + AVV / T
4510 QL = QN
4520 Next x
4530 GoTo 3600
4540 FAC = 1
4550 L1 = Z1
4560 L2 = Z2
4570 For x = 1 To NPA
4580 FAC = 1
4590 QL = QN
4600 QN = 0
4610 QP = 0
4620 T = T + dela
4630 YY = -DXX
4640 YY = YY + 2 * DXX
4650 WW = 0
4660 If IFLG = 0 Then GoTo 4720
4670 U = YY / ((4 * TR * fact * T / (S * 7.481)) ^ 0.5)
4680 GoSub 5490
4690 WW = WW + BQQ
4700 QQ(x) = BQQ * FAC / 2 + QQ(x)
4710 VV(x) = VV(x) + SVV * FAC / 2
4720 U = 4 * TR * T * fact / (S * 7.481)
4730 If L1 <> L2 Then GoSub 7130 Else GoTo 4810
4740 WW = WW + BQQ
4750 QQ(x) = QQ(x) + BQQ * FAC
4760 QN = QN + BQQ * FAC
4770 U = 4 * TR * (T - dela / 2) * fact / (S * 7.481)
4780 XYZ = U
4790 If L1 <> L2 Then GoSub 7130
4800 QP = QP + BQQ * FAC
4810 YY = YY - 2 * DXX + 2 * W
4820 If IFLG = 0 Then GoTo 4880
4830 U = YY / ((4 * TR * fact * T / (S * 7.481)) ^ 0.5)
4840 GoSub 5490
4850 WW = WW + BQQ
4860 QQ(x) = QQ(x) + BQQ * FAC / 2
4870 VV(x) = VV(x) + SVV * FAC / 2
4880 U = XYZ
4890 If L1 <> L2 Then GoSub 7130 Else GoTo 4960
4900 QP = QP + BQQ * FAC
4910 U = 4 * TR * T * fact / (S * 7.481)
4920 If L1 <> L2 Then GoSub 7130
4930 WW = WW + BQQ
4940 QN = QN + BQQ * FAC
4950 QQ(x) = QQ(x) + BQQ * FAC
4960 If Abs(WW) < 0.00001 Then GoTo 4990
4970 FAC = FAC * (-1)
4980 GoTo 4640
4990 If L1 <> L2 Then GoSub 7300 Else GoTo 5020
5000 AVV = AVV + SVV
5010 VV(x) = VV(x) + AVV / T
5020 Next x
5030 GoTo 3600
5040 For x = 1 To NPA
5050 T = T + dela
5060 U = YY / ((4 * TR * fact * T / (S * 7.481)) ^ 0.5)
5070 If U > 2.9 Then GoTo 5460
5080 If IFLG = 0 Then GoTo 5260
5090 GoSub 5490
5100 QQ(x) = BQQ
5110 VV(x) = SVV
5120 If B = Z2 Then GoTo 5470
5130 L1 = Z2
5140 L2 = 2 * B - Z2
5150 U = 4 * TR * T * fact / (S * 7.481)
5160 GoSub 7130
5170 QQ(x) = QQ(x) - BQQ
5180 QN = BQQ
5190 U = 4 * TR * (T = dela / 2) * fact / (S * 7.481)
5200 GoSub 7130
5210 QP = BQQ
5220 GoSub 7300
5230 AVV = AVV + SVV
5240 VV(x) = VV(x) - AVV / T
5250 GoTo 5460
5260 L1 = Z1
5270 L2 = Z2
5280 U = 4 * TR * T * fact / (S * 7.481)
5290 GoSub 7130
5300 QQ(x) = BQQ
5310 QN = BQQ
5320 U = 4 * TR * (T - dela / 2) * fact / (S * 7.481)
5330 GoSub 7130
5340 QP = BQQ
5350 L1 = 2 * B - Z2
5360 L2 = 2 * B - Z1
5370 GoSub 7130
5380 QP = BQQ + QP
5390 U = 4 * TR * T * fact / (S * 7.481)
5400 GoSub 7130
5410 QN = QN + BQQ
5420 QQ(x) = QQ(x) + BQQ
5430 GoSub 7300
5440 AVV = AVV + SVV
5450 VV(x) = AVV / T
5460 QL = QN
5470 Next x
5480 GoTo 3600
5490 '********* SUB TO EVALUATE COMPLIMENTARY ERROR FUNCTION AND 2nd REPEATED
5500 '********* INTEGRAL OF COMPLIMENTARY ERROR FUNCTION ***********************
5510 If U > 2.9 Then BQQ = 0: SVV = 0: Return
5520 Sum = U
5530 M = 0
5540 N = U ^ 2
5550 TERM = U
5560 P = 1
5570 M = M + 1
5580 P = P + 2
5590 TERM = ((-1) * TERM * N * (P - 2)) / (P * M)
5600 Sum = Sum + TERM
5610 If (Abs(TERM)) > 0.00000001 Then GoTo 5570
5620 BQQ = 1 - 2 * Sum / (PI ^ 0.5)
5630 SVV = BQQ * (1 + 2 * (U ^ 2)) - (2 * U * E ^ -(U ^ 2)) / (PI ^ 0.5)
5640 Return
5650 '************************ DRAW GRAPH **************************************
5660 MAXVD = -1000000!
5670 MAXQD = -1000000!
5680 MINVD = 1000000!
5690 MINQD = 1000000!
5700 Print #3, Chr$(15);
5710 Print #3, Chr$(27); "1";
5720 For x = 1 To NPA
5730 If qd(x) > MAXQD Then MAXQD = qd(x)
5740 If qd(x) < MINQD Then MINQD = qd(x)
5750 If vd(x) > MAXVD Then MAXVD = vd(x)
5760 If vd(x) < MINVD Then MINVD = vd(x)
5770 Next x
5780 If MINVD > 0 Then MINVD = 0
5790 If MINQD > 0 Then MINQD = 0
5800 LVD = MAXVD - MINVD
5810 LQD = MAXQD - MINQD
5820 LBP = Int(50 / NPA) + 1
5830 Print #3,
5840 Print #3,
5850 Print #3,
5860 Print #3, Tab(20); "STREAM DEPLETION VS TIME ("; TU$(TUI); ")"
5870 Print #3,
5880 Print #3,
5890 Print #3, Tab(10); "***** RATE OF DEPLETION (GPM)"
5900 Print #3,
5910 For IZ = 1 To 6
5920 If IZ <> 1 Then Print #3, Tab(IZ * 20 - 19);
5930 Print #3, USING; "##########.##"; MINQD + (IZ - 1) * LQD / 5;
5940 Next IZ
5950 Print #3,
5960 Print #3, Tab(10); "+-------------------+-------------------+-------------------+-------------------+-------------------+"
5970 Print #3,
5980 Print #3,
5990 Print #3, Tab(10); "+++++ VOL. OF DEPLETION ("; VUNIT$; ")"
6000 Print #3,
6010 For IZ = 1 To 6
6020 If IZ <> 1 Then Print #3, Tab(IZ * 20 - 19);
6030 Print #3, USING; "##########.##"; MINVD * VFAC + (IZ - 1) * VFAC * LVD / 5;
6040 Next IZ
6050 Print #3,
6060 Print #3, Tab(7); "0  +-------------------+-------------------+-------------------+-------------------+-------------------+"
6070 For x = 1 To NPA
6080 qd(x) = (qd(x) - MINQD) * 100 / LQD + 10
6090 vd(x) = (vd(x) - MINVD) * 100 / LVD + 10
6100 If LBP = 1 Then GoTo 6140
6110 For J = 1 To (LBP - 1)
6120 Print #3, Tab(10); "!"
6130 Next J
6140 If CInt(vd(x)) > CInt(qd(x)) Then GoTo 6380
6150 If CInt(vd(x)) = CInt(qd(x)) Then GoTo 6270
6160 If (x / 5 - Int(x / 5)) = 0 Then GoTo 6220
6170 If vd(x) < 10.5 Then GoTo 6200
6180 Print #3, Tab(10); "!"; Tab(vd(x)); "+"; Tab(qd(x)); "*"
6190 GoTo 6480
6200 Print #3, Tab(10); "+"; Tab(qd(x)); "*"
6210 GoTo 6480
6220 If vd(x) < 10.5 Then GoTo 6250
6230 Print #3, Tab(5); (x * dela); Tab(10); "+"; Tab(vd(x)); "+"; Tab(qd(x)); "*"
6240 GoTo 6480
6250 Print #3, Tab(5); (x * dela); Tab(10); "+"; Tab(qd(x)); "*"
6260 GoTo 6480
6270 If (x / 5 - Int(x / 5)) = 0 Then GoTo 6330
6280 If qd(x) < 10.5 Then GoTo 6310
6290 Print #3, Tab(10); "!"; Tab(qd(x)); "*"
6300 GoTo 6480
6310 Print #3, Tab(10); "*"
6320 GoTo 6480
6330 If qd(x) < 10.5 Then GoTo 6360
6340 Print #3, Tab(5); (x * dela); Tab(10); "!"; Tab(qd(x)); "*"
6350 GoTo 6480
6360 Print #3, Tab(5); (x * dela); Tab(10); "*"
6370 GoTo 6480
6380 If (x / 5 - Int(x / 5)) = 0 Then GoTo 6440
6390 If qd(x) < 10.5 Then GoTo 6420
6400 Print #3, Tab(10); "!"; Tab(qd(x)); "*"; Tab(vd(x)); "+"
6410 GoTo 6480
6420 Print #3, Tab(10); "*"; Tab(vd(x)); "+"
6430 GoTo 6480
6440 If qd(x) < 10.5 Then GoTo 6470
6450 Print #3, Tab(5); (x * dela); Tab(10); "+"; Tab(qd(x)); "*"; Tab(vd(x)); "+"
6460 GoTo 6480
6470 Print #3, Tab(5); (x * dela); Tab(10); "*"; Tab(vd(x)); "+"
6480 Next x
6490 Print #3,
6500 Print #3, Tab(10); "+-------------------+-------------------+-------------------+-------------------+-------------------+"
6510 Print #3,
6520 Print #3, "IF RATE AND VOLUME HAVE SAME COORDINATES ONLY AN (*) IS PRINTED."
6530 Print #3, Chr$(27); "@";
6540 End
6550 '********** SUBROUTINE FOR PRINTING PUMPING SCHEDULE *********************
6560 Print #3,
6570 Print #3,
6580 Print #3,
6590 Print #3, Tab(31); "PUMPING SCHEDULE"
6600 Print #3, Tab(23); "TOTAL TIME SIMULATED="; TT; TU$(TUI)
6610 Print #3,
6620 Print #3, "                                                    VOL. PUMPED     CUM. VOL."
6630 Print #3, "PUMPING           Q          LENGTH     CUM. TIME   THIS PERIOD      PUMPED"
6640 ZZ$ = "(" + TU$(TUI) + ")": For II = 1 To (14 - Len(ZZ$)): ZZ$ = " " + ZZ$: Next II
6650 ZZZ$ = "(" + VUNIT$ + ")": For II = 1 To (14 - Len(ZZZ$)): ZZZ$ = " " + ZZZ$: Next II
6660 Print #3, "PERIOD          (GPM)"; ZZ$; ZZ$; ZZZ$; ZZZ$
6670 Print #3,
6680 For x = 1 To np
6690 Print #3, USING; "####"; x;: Print #3, Tab(8);
6700 Print #3, USING; "##########.###"; q(x);
6710 Print #3, USING; "##############"; DELT(x); ZZAT(x);
6720 Print #3, USING; "##########.###"; VPTP(x); AVP(x)
6730 Next x
6740 Print #3,
6750 Print #3,
6760 Print #3,
6770 Print #3,
6780 Return
6790 '************** SUBROUTINE FOR DETERMINING VOL. PUMPED AND VOL. UNITS ****
6800 For x = 1 To np
6810 VPTP(x) = q(x) * DELT(x) * fact * 1440
6820 AVSUM = AVSUM + VPTP(x)
6830 AVP(x) = AVSUM
6840 Next x
6850 If (Abs(AVSUM)) > 1000000! Then GoTo 6880
6860 VUNIT$ = "GALLONS"
6870 Return
6880 For x = 1 To np
6890 VPTP(x) = VPTP(x) / 325900!
6900 AVP(x) = AVP(x) / 325900!
6910 Next x
6920 VUNIT$ = "ACRE-FEET"
6930 Return
6940 '*********** SUBROUTINE TO INPUT CYCLIC PUMPING SCHEDULE ******************
7130 '*************** SUBROUTINE FOR q/Q (STREAM SEGMENT) **********************
7140 SIMN = Abs(Int((L2 - L1) / YY)) * 20
7150 If SIMN < 40 Then SIMN = 40
7160 If SIMN > 100 Then SIMN = 100
7170 DELX = (L2 - L1) / SIMN
7180 XC = L1 - DELX
7190 For N = 1 To (SIMN + 1)
7200 XC = XC + DELX
7210 AB = (YY ^ 2) + (XC ^ 2)
7220 If (AB / U) > 60 Then H(N) = 0 Else H(N) = (E ^ (-(AB / U))) / AB
7230 Next N
7240 BQQ = H(1) + H(SIMN + 1)
7250 For N = 2 To SIMN
7260 If (N / 2 - Int(N / 2)) = 0 Then BQQ = BQQ + 4 * H(N) Else BQQ = BQQ + 2 * H(N)
7270 Next N
7280 If BQQ < 1E-33 Then BQQ = 0 Else BQQ = BQQ * DELX * YY / (3 * 3.1415927)
7290 Return
7300 '**************** SUBROUTINE FOR v/V (STREAM SEGMENT) *********************
7310 SVV = ((QN + QL) * dela / 2 + (QP - (QL + QN) / 2) * dela * 2 / 3)
7320 Return
8000 If Err = 53 Then Print #3, "no files currently on disk": Resume 8090
Print #3, "disk problem": opt5 = 1: Return
Rem   open file to save results on disk
8020 Rem
Rem PRINT "Directory of drive B": FILES "b:*.*"
8090 Print "Enter file name for output file"

Print #3, opt5, head1$
Print #3, "step     q/Q                v/V"
Return
8200 Rem   print results to file
Print #3, dela * x; qd(x) / q(1); vd(x) / (q(1) * dela * x * fact)
Return
End

End Sub

Private Sub Form_Load()
  seodep2
End Sub
