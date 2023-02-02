//# 17 inputs
//# 5 outputs
//# 74 D-type flipflops
//# 167 inverters
//# 490 gates (197 ANDs + 64 NANDs + 137 ORs + 92 NORs)

module dff (CK,Q,D);
input CK,D;
output Q;
  always @(posedge CK) Q <= D;

endmodule

module s1423a(GND,VDD,CK,G0,G1,G10,G11,G12,G13,G14,G15,G16,G2,G3,G4,G5,G6,G7,
  G701BF,G702,G726,G727,G729,G8,G9);
input GND,VDD,CK,G0,G1,G2,G3,G4,G5,G6,G7,G8,G9,G10,G11,G12,G13,G14,G15,G16;
output G726,G729,G702,G727,G701BF;

  wire G22,G332BF,G23,G328BF,G24,G109,G25,G113,G26,G118,G27,G125,G28,G129,G29,
    G140,G30,G144,G31,G149,G32,G154,G33,G159,G34,G166,G35,G175,G36,G189,G37,
    G193,G38,G198,G39,G208,G40,G214,G41,G218,G42,G237,G43,G242,G44,G247,G45,
    G252,G46,G260,G47,G303,G48,G309,G49,G315,G50,G321,G51,G360,G52,G365,G53,
    G373,G54,G379,G55,G384,G56,G392,G57,G397,G58,G405,G59,G408,G60,G416,G61,
    G424,G62,G427,G63,G438,G64,G441,G65,G447,G66,G451,G67,G459,G68,G464,G69,
    G469,G70,G477,G71,G494,G72,G498,G73,G503,G74,G526,G75,G531,G76,G536,G77,
    G541,G78,G548,G79,G565,G80,G569,G81,G573,G82,G577,G83,G590,G84,G608,G85,
    G613,G86,G657,G87,G663,G88,G669,G89,G675,G90,G682,G91,G687,G92,G693,G93,
    G705,G94,G707,G95,G713,II1,G332,II12,G328,G108,G712,G111,G112,G117,G124,
    G127,G128,G139,G142,G143,G148,G153,G158,G165,G174,G176,G178,G179,G180,G188,
    G191,G192,G197,G204,G207,G210,G213,G216,G217,G236,G259,G241,G246,G251,G258,
    G296,G297,G302,G305,G324,G308,G311,G314,G317,G320,G323,G336,G355,G339,G343,
    G348,G347,G351,G645,G354,G359,G372,G364,G371,G378,G391,G383,G390,G396,G404,
    G403,G407,G415,G423,G422,G426,G437,G440,G445,G446,G449,G450,G455,G456,G458,
    G476,G463,G468,G475,G486,G491,G500,G495,G499,G504,G511,G507,G510,G525,G589,
    G530,G535,G540,G547,G562,G610,G566,G570,G574,G588,G595,G593,G596,G597,G600,
    G601,G605,G609,G614,G615,G616,G617,G620,G623,G626,G629,G632,G635,G638,G641,
    G644,G656,G658,G659,II1162,G661,G662,G665,G678,G668,G671,G674,G677,II1183,
    G685,G696,G689,G695,II1203,G701,II1211,G704,G706,G711,G714,II1227,G715,
    II1230,G716,II1233,G717,II1236,G718,II1239,G719,II1242,G720,II1245,G721,
    II1248,G722,II1251,G723,II1254,G724,II1257,G725,II1260,II1264,G728,II1267,
    G101,G630,G631,G102,G633,G634,G103,G636,G637,G104,G639,G640,G105,G642,G643,
    G106,G114,G116,G133,G119,G121,G134,G122,G130,G132,G136,G700,G135,G137,G145,
    G147,G168,G150,G152,G169,G155,G157,G170,G160,G162,G171,G163,G177,G172,G173,
    G185,G181,G182,G186,G194,G196,G202,G199,G201,G203,G522,G205,G211,G219,G221,
    G223,G222,G183,G224,G225,G226,G227,G228,G229,G432,G238,G240,G299,G243,G245,
    G262,G248,G250,G263,G253,G255,G264,G624,G625,G256,G261,G265,G271,G275,G266,
    G272,G276,G277,G273,G278,G279,G274,G280,G281,G304,G306,G307,G310,G312,G313,
    G316,G318,G319,G322,G325,G326,G329,G331,G330,G335,G337,G338,G342,G344,G345,
    G346,G349,G350,G358,G523,G361,G363,G366,G368,G375,G369,G374,G376,G377,G380,
    G382,G385,G387,G394,G388,G393,G395,G398,G400,G401,G406,G412,G409,G411,G413,
    G414,G417,G419,G420,G425,G431,G428,G430,G433,G356,G357,G435,G340,G341,G436,
    G352,G353,G439,G442,G443,G448,G452,G453,G457,G460,G462,G434,G465,G467,G479,
    G470,G472,G480,G473,G478,G481,G488,G505,G506,G489,G508,G509,G490,G512,G513,
    G492,G493,G496,G497,G501,G502,G527,G529,G604,G532,G534,G550,G537,G539,G551,
    G542,G544,G552,G545,G549,G553,G563,G564,G567,G568,G571,G572,G575,G576,G627,
    G628,G591,G592,G594,G621,G622,G524,G606,G607,G611,G612,G648,G646,G647,G649,
    G618,G619,G650,G651,G652,G653,G654,G655,G664,G666,G667,G670,G672,G673,G676,
    G679,G680,G683,G684,G688,G690,G691,G694,G697,G698,G703,G230,G708,G709,G599,
    G110,G126,G141,G167,G184,G190,G209,G215,G235,G233,G267,G268,G269,G282,G283,
    G270,G291,G292,G293,G294,G295,G300,G333,G334,G301,G518,G519,G520,G521,G487,
    G554,G555,G583,G584,G585,G586,G587,G561,G602,G603,G96,G97,G98,G99,G100,
    G681,G699,G686,G692,G107,G123,G138,G164,G187,G206,G212,G234,G231,G232,G298,
    G286,G287,G288,G284,G285,G289,G290,G482,G514,G483,G515,G484,G516,G485,G517,
    G556,G557,G558,G559,G560,G578,G579,G580,G581,G582,G598,G115,G120,G131,G146,
    G151,G156,G161,G195,G200,G220,G239,G244,G249,G254,G257,G327,G362,G367,G370,
    G381,G386,G389,G399,G402,G410,G418,G421,G429,G444,G454,G461,G466,G471,G474,
    G528,G533,G538,G543,G546,G660,G710;

  dff DFF_0(CK,G22,G332BF);
  dff DFF_1(CK,G23,G328BF);
  dff DFF_2(CK,G24,G109);
  dff DFF_3(CK,G25,G113);
  dff DFF_4(CK,G26,G118);
  dff DFF_5(CK,G27,G125);
  dff DFF_6(CK,G28,G129);
  dff DFF_7(CK,G29,G140);
  dff DFF_8(CK,G30,G144);
  dff DFF_9(CK,G31,G149);
  dff DFF_10(CK,G32,G154);
  dff DFF_11(CK,G33,G159);
  dff DFF_12(CK,G34,G166);
  dff DFF_13(CK,G35,G175);
  dff DFF_14(CK,G36,G189);
  dff DFF_15(CK,G37,G193);
  dff DFF_16(CK,G38,G198);
  dff DFF_17(CK,G39,G208);
  dff DFF_18(CK,G40,G214);
  dff DFF_19(CK,G41,G218);
  dff DFF_20(CK,G42,G237);
  dff DFF_21(CK,G43,G242);
  dff DFF_22(CK,G44,G247);
  dff DFF_23(CK,G45,G252);
  dff DFF_24(CK,G46,G260);
  dff DFF_25(CK,G47,G303);
  dff DFF_26(CK,G48,G309);
  dff DFF_27(CK,G49,G315);
  dff DFF_28(CK,G50,G321);
  dff DFF_29(CK,G51,G360);
  dff DFF_30(CK,G52,G365);
  dff DFF_31(CK,G53,G373);
  dff DFF_32(CK,G54,G379);
  dff DFF_33(CK,G55,G384);
  dff DFF_34(CK,G56,G392);
  dff DFF_35(CK,G57,G397);
  dff DFF_36(CK,G58,G405);
  dff DFF_37(CK,G59,G408);
  dff DFF_38(CK,G60,G416);
  dff DFF_39(CK,G61,G424);
  dff DFF_40(CK,G62,G427);
  dff DFF_41(CK,G63,G438);
  dff DFF_42(CK,G64,G441);
  dff DFF_43(CK,G65,G447);
  dff DFF_44(CK,G66,G451);
  dff DFF_45(CK,G67,G459);
  dff DFF_46(CK,G68,G464);
  dff DFF_47(CK,G69,G469);
  dff DFF_48(CK,G70,G477);
  dff DFF_49(CK,G71,G494);
  dff DFF_50(CK,G72,G498);
  dff DFF_51(CK,G73,G503);
  dff DFF_52(CK,G74,G526);
  dff DFF_53(CK,G75,G531);
  dff DFF_54(CK,G76,G536);
  dff DFF_55(CK,G77,G541);
  dff DFF_56(CK,G78,G548);
  dff DFF_57(CK,G79,G565);
  dff DFF_58(CK,G80,G569);
  dff DFF_59(CK,G81,G573);
  dff DFF_60(CK,G82,G577);
  dff DFF_61(CK,G83,G590);
  dff DFF_62(CK,G84,G608);
  dff DFF_63(CK,G85,G613);
  dff DFF_64(CK,G86,G657);
  dff DFF_65(CK,G87,G663);
  dff DFF_66(CK,G88,G669);
  dff DFF_67(CK,G89,G675);
  dff DFF_68(CK,G90,G682);
  dff DFF_69(CK,G91,G687);
  dff DFF_70(CK,G92,G693);
  dff DFF_71(CK,G93,G705);
  dff DFF_72(CK,G94,G707);
  dff DFF_73(CK,G95,G713);
  not NOT_0(II1,G332);
  not NOT_1(G332BF,II1);
  not NOT_2(II12,G328);
  not NOT_3(G328BF,II12);
  not NOT_4(G108,G712);
  not NOT_5(G111,G24);
  not NOT_6(G112,G712);
  not NOT_7(G117,G712);
  not NOT_8(G124,G712);
  not NOT_9(G127,G27);
  not NOT_10(G128,G712);
  not NOT_11(G139,G712);
  not NOT_12(G142,G29);
  not NOT_13(G143,G712);
  not NOT_14(G148,G712);
  not NOT_15(G153,G712);
  not NOT_16(G158,G712);
  not NOT_17(G165,G712);
  not NOT_18(G174,G712);
  not NOT_19(G176,G35);
  not NOT_20(G178,G34);
  not NOT_21(G179,G180);
  not NOT_22(G180,G92);
  not NOT_23(G188,G712);
  not NOT_24(G191,G36);
  not NOT_25(G192,G712);
  not NOT_26(G197,G712);
  not NOT_27(G204,G38);
  not NOT_28(G207,G712);
  not NOT_29(G210,G39);
  not NOT_30(G213,G712);
  not NOT_31(G216,G40);
  not NOT_32(G217,G712);
  not NOT_33(G236,G259);
  not NOT_34(G241,G259);
  not NOT_35(G246,G259);
  not NOT_36(G251,G259);
  not NOT_37(G258,G259);
  not NOT_38(G296,G297);
  not NOT_39(G302,G712);
  not NOT_40(G305,G324);
  not NOT_41(G308,G712);
  not NOT_42(G311,G324);
  not NOT_43(G314,G712);
  not NOT_44(G317,G324);
  not NOT_45(G320,G712);
  not NOT_46(G323,G324);
  not NOT_47(G336,G355);
  not NOT_48(G339,G355);
  not NOT_49(G343,G348);
  not NOT_50(G347,G348);
  not NOT_51(G348,G91);
  not NOT_52(G351,G645);
  not NOT_53(G354,G355);
  not NOT_54(G359,G372);
  not NOT_55(G364,G372);
  not NOT_56(G371,G372);
  not NOT_57(G378,G391);
  not NOT_58(G383,G391);
  not NOT_59(G390,G391);
  not NOT_60(G396,G404);
  not NOT_61(G403,G404);
  not NOT_62(G407,G712);
  not NOT_63(G415,G423);
  not NOT_64(G422,G423);
  not NOT_65(G426,G712);
  not NOT_66(G437,G712);
  not NOT_67(G440,G712);
  not NOT_68(G445,G65);
  not NOT_69(G446,G712);
  not NOT_70(G449,G66);
  not NOT_71(G450,G712);
  not NOT_72(G455,G456);
  not NOT_73(G458,G476);
  not NOT_74(G463,G476);
  not NOT_75(G468,G476);
  not NOT_76(G475,G476);
  not NOT_77(G486,G712);
  not NOT_78(G491,G500);
  not NOT_79(G495,G500);
  not NOT_80(G499,G500);
  not NOT_81(G504,G511);
  not NOT_82(G507,G511);
  not NOT_83(G510,G511);
  not NOT_84(G511,G63);
  not NOT_85(G525,G589);
  not NOT_86(G530,G589);
  not NOT_87(G535,G589);
  not NOT_88(G540,G589);
  not NOT_89(G547,G589);
  not NOT_90(G562,G610);
  not NOT_91(G566,G610);
  not NOT_92(G570,G610);
  not NOT_93(G574,G610);
  not NOT_94(G588,G589);
  not NOT_95(G595,G593);
  not NOT_96(G596,G597);
  not NOT_97(G600,G601);
  not NOT_98(G605,G610);
  not NOT_99(G609,G610);
  not NOT_100(G614,G64);
  not NOT_101(G615,G616);
  not NOT_102(G617,G645);
  not NOT_103(G620,G645);
  not NOT_104(G623,G645);
  not NOT_105(G626,G645);
  not NOT_106(G629,G645);
  not NOT_107(G632,G645);
  not NOT_108(G635,G645);
  not NOT_109(G638,G645);
  not NOT_110(G641,G645);
  not NOT_111(G644,G645);
  not NOT_112(G645,G90);
  not NOT_113(G656,G712);
  not NOT_114(G658,G659);
  not NOT_115(II1162,G13);
  not NOT_116(G659,II1162);
  not NOT_117(G661,G94);
  not NOT_118(G662,G712);
  not NOT_119(G665,G678);
  not NOT_120(G668,G712);
  not NOT_121(G671,G678);
  not NOT_122(G674,G712);
  not NOT_123(G677,G678);
  not NOT_124(II1183,G11);
  not NOT_125(G678,II1183);
  not NOT_126(G685,G696);
  not NOT_127(G689,G696);
  not NOT_128(G695,G696);
  not NOT_129(II1203,G10);
  not NOT_130(G696,II1203);
  not NOT_131(G701,G15);
  not NOT_132(II1211,G701);
  not NOT_133(G701BF,II1211);
  not NOT_134(G704,G712);
  not NOT_135(G706,G712);
  not NOT_136(G711,G712);
  not NOT_137(G712,G14);
  not NOT_138(G714,G701);
  not NOT_139(II1227,G6);
  not NOT_140(G715,II1227);
  not NOT_141(II1230,G7);
  not NOT_142(G716,II1230);
  not NOT_143(II1233,G8);
  not NOT_144(G717,II1233);
  not NOT_145(II1236,G9);
  not NOT_146(G718,II1236);
  not NOT_147(II1239,G12);
  not NOT_148(G719,II1239);
  not NOT_149(II1242,G0);
  not NOT_150(G720,II1242);
  not NOT_151(II1245,G1);
  not NOT_152(G721,II1245);
  not NOT_153(II1248,G2);
  not NOT_154(G722,II1248);
  not NOT_155(II1251,G3);
  not NOT_156(G723,II1251);
  not NOT_157(II1254,G4);
  not NOT_158(G724,II1254);
  not NOT_159(II1257,G5);
  not NOT_160(G725,II1257);
  not NOT_161(II1260,G93);
  not NOT_162(G726,II1260);
  not NOT_163(II1264,G16);
  not NOT_164(G728,II1264);
  not NOT_165(II1267,G95);
  not NOT_166(G729,II1267);
  and AND2_0(G101,G630,G631);
  and AND2_1(G102,G633,G634);
  and AND2_2(G103,G636,G637);
  and AND2_3(G104,G639,G640);
  and AND2_4(G105,G642,G643);
  and AND2_5(G109,G106,G108);
  and AND2_6(G113,G114,G112);
  and AND2_7(G116,G133,G25);
  and AND2_8(G118,G119,G117);
  and AND2_9(G121,G134,G26);
  and AND2_10(G125,G122,G124);
  and AND2_11(G129,G130,G128);
  and AND2_12(G132,G136,G28);
  and AND2_13(G133,G700,G111);
  and AND2_14(G134,G133,G25);
  and AND2_15(G135,G134,G26);
  and AND2_16(G136,G135,G127);
  and AND2_17(G140,G137,G139);
  and AND2_18(G144,G145,G143);
  and AND2_19(G147,G168,G30);
  and AND2_20(G149,G150,G148);
  and AND2_21(G152,G169,G31);
  and AND2_22(G154,G155,G153);
  and AND2_23(G157,G170,G32);
  and AND2_24(G159,G160,G158);
  and AND2_25(G162,G171,G33);
  and AND2_26(G166,G163,G165);
  and AND2_27(G168,G177,G142);
  and AND2_28(G169,G168,G30);
  and AND2_29(G170,G169,G31);
  and AND2_30(G171,G170,G32);
  and AND2_31(G172,G171,G33);
  and AND2_32(G173,G172,G34);
  and AND2_33(G175,G176,G174);
  and AND2_34(G185,G181,G182);
  and AND2_35(G189,G186,G188);
  and AND2_36(G193,G194,G192);
  and AND2_37(G196,G202,G37);
  and AND2_38(G198,G199,G197);
  and AND2_39(G201,G203,G38);
  and AND2_40(G202,G522,G191);
  and AND2_41(G203,G202,G37);
  and AND2_42(G208,G205,G207);
  and AND2_43(G214,G211,G213);
  and AND2_44(G218,G219,G217);
  and AND2_45(G221,G223,G41);
  and AND2_46(G222,G183,G210);
  and AND2_47(G223,G222,G216);
  and AND2_48(G224,G203,G38);
  and AND2_49(G225,G204,G203);
  and AND2_50(G226,G136,G28);
  and AND2_51(G227,G172,G178);
  and AND2_52(G228,G223,G41);
  and AND2_53(G229,G432,G62);
  and AND2_54(G237,G238,G236);
  and AND2_55(G240,G299,G42);
  and AND2_56(G242,G243,G241);
  and AND2_57(G245,G262,G43);
  and AND2_58(G247,G248,G246);
  and AND2_59(G250,G263,G44);
  and AND2_60(G252,G253,G251);
  and AND2_61(G255,G264,G45);
  and AND2_62(G259,G624,G625);
  and AND2_63(G260,G256,G258);
  and AND2_64(G261,G265,G46);
  and AND2_65(G262,G299,G42);
  and AND2_66(G263,G262,G43);
  and AND2_67(G264,G263,G44);
  and AND2_68(G265,G264,G45);
  and AND2_69(G271,G275,G266);
  and AND2_70(G272,G276,G277);
  and AND2_71(G273,G278,G279);
  and AND2_72(G274,G280,G281);
  and AND2_73(G303,G304,G302);
  and AND2_74(G304,G306,G307);
  and AND2_75(G309,G310,G308);
  and AND2_76(G310,G312,G313);
  and AND2_77(G315,G316,G314);
  and AND2_78(G316,G318,G319);
  and AND2_79(G321,G322,G320);
  and AND2_80(G322,G325,G326);
  and AND2_81(G329,G331,G714);
  and AND2_82(G330,G332,G714);
  and AND2_83(G335,G337,G338);
  and AND2_84(G342,G344,G345);
  and AND2_85(G346,G349,G350);
  and AND2_86(G358,G523,G53);
  and AND2_87(G360,G361,G359);
  and AND2_88(G363,G523,G51);
  and AND2_89(G365,G366,G364);
  and AND2_90(G368,G375,G52);
  and AND2_91(G373,G369,G371);
  and AND2_92(G374,G376,G53);
  and AND2_93(G375,G523,G51);
  and AND2_94(G376,G375,G52);
  and AND3_0(G377,G183,G54,G56);
  and AND2_95(G379,G380,G378);
  and AND2_96(G382,G183,G54);
  and AND2_97(G384,G385,G383);
  and AND2_98(G387,G394,G55);
  and AND2_99(G392,G388,G390);
  and AND2_100(G393,G395,G56);
  and AND2_101(G394,G183,G54);
  and AND2_102(G395,G394,G55);
  and AND2_103(G397,G398,G396);
  and AND2_104(G400,G335,G57);
  and AND2_105(G405,G401,G403);
  and AND2_106(G406,G412,G58);
  and AND2_107(G408,G409,G407);
  and AND2_108(G411,G413,G59);
  and AND2_109(G412,G335,G57);
  and AND2_110(G413,G335,G58);
  and AND2_111(G414,G413,G59);
  and AND2_112(G416,G417,G415);
  and AND2_113(G419,G358,G60);
  and AND2_114(G424,G420,G422);
  and AND2_115(G425,G431,G61);
  and AND2_116(G427,G428,G426);
  and AND2_117(G430,G432,G62);
  and AND2_118(G431,G358,G60);
  and AND2_119(G432,G358,G61);
  and AND2_120(G433,G356,G357);
  and AND2_121(G435,G340,G341);
  and AND2_122(G436,G352,G353);
  and AND2_123(G438,G439,G437);
  and AND2_124(G441,G442,G440);
  and AND2_125(G443,G615,G511);
  and AND2_126(G447,G448,G446);
  and AND2_127(G451,G452,G450);
  and AND2_128(G453,G615,G445);
  and AND3_1(G457,G455,G449,G728);
  and AND2_129(G459,G460,G458);
  and AND2_130(G462,G434,G67);
  and AND2_131(G464,G465,G463);
  and AND2_132(G467,G479,G68);
  and AND2_133(G469,G470,G468);
  and AND2_134(G472,G480,G69);
  and AND2_135(G477,G473,G475);
  and AND2_136(G478,G481,G70);
  and AND2_137(G479,G434,G67);
  and AND2_138(G480,G479,G68);
  and AND2_139(G481,G480,G69);
  and AND2_140(G488,G505,G506);
  and AND2_141(G489,G508,G509);
  and AND2_142(G490,G512,G513);
  and AND2_143(G494,G492,G493);
  and AND2_144(G498,G496,G497);
  and AND2_145(G503,G501,G502);
  and AND2_146(G526,G527,G525);
  and AND2_147(G529,G604,G74);
  and AND2_148(G531,G532,G530);
  and AND2_149(G534,G550,G75);
  and AND2_150(G536,G537,G535);
  and AND2_151(G539,G551,G76);
  and AND2_152(G541,G542,G540);
  and AND2_153(G544,G552,G77);
  and AND2_154(G548,G545,G547);
  and AND2_155(G549,G553,G78);
  and AND2_156(G550,G604,G74);
  and AND2_157(G551,G550,G75);
  and AND2_158(G552,G551,G76);
  and AND2_159(G553,G552,G77);
  and AND2_160(G565,G563,G564);
  and AND2_161(G569,G567,G568);
  and AND2_162(G573,G571,G572);
  and AND2_163(G577,G575,G576);
  and AND2_164(G589,G627,G628);
  and AND2_165(G590,G591,G588);
  and AND2_166(G592,G594,G595);
  and AND2_167(G601,G621,G622);
  and AND2_168(G604,G433,G524);
  and AND2_169(G608,G606,G607);
  and AND2_170(G613,G611,G612);
  and AND2_171(G648,G646,G647);
  and AND2_172(G649,G618,G619);
  and AND2_173(G650,G226,G661);
  and AND2_174(G651,G227,G87);
  and AND2_175(G652,G228,G88);
  and AND2_176(G653,G229,G89);
  and AND2_177(G654,G90,G476);
  and AND2_178(G655,G91,G476);
  and AND2_179(G657,G659,G656);
  and AND2_180(G663,G664,G662);
  and AND2_181(G664,G666,G667);
  and AND2_182(G669,G670,G668);
  and AND2_183(G670,G672,G673);
  and AND2_184(G675,G676,G674);
  and AND2_185(G676,G679,G680);
  and AND2_186(G683,G684,G685);
  and AND2_187(G688,G690,G691);
  and AND2_188(G694,G697,G698);
  and AND2_189(G702,G703,G645);
  and AND2_190(G705,G230,G704);
  and AND2_191(G707,G708,G706);
  and AND2_192(G709,G678,G89);
  and AND2_193(G713,G599,G711);
  and AND2_194(G727,G476,G645);
  or OR2_0(G110,G700,G111);
  or OR2_1(G126,G135,G127);
  or OR2_2(G141,G177,G142);
  or OR2_3(G167,G172,G178);
  or OR2_4(G177,G180,G226);
  or OR2_5(G181,G178,G180);
  or OR2_6(G182,G35,G179);
  or OR2_7(G183,G180,G227);
  or OR2_8(G184,G180,G173);
  or OR2_9(G190,G522,G191);
  or OR2_10(G209,G183,G210);
  or OR2_11(G215,G222,G216);
  or OR2_12(G235,G649,G233);
  or OR2_13(G275,G101,G42);
  or OR2_14(G276,G102,G43);
  or OR2_15(G277,G267,G271);
  or OR2_16(G278,G103,G44);
  or OR2_17(G279,G268,G272);
  or OR2_18(G280,G104,G45);
  or OR2_19(G281,G269,G273);
  or OR2_20(G282,G105,G46);
  or OR2_21(G283,G270,G274);
  or OR2_22(G291,G42,G101);
  or OR2_23(G292,G43,G102);
  or OR2_24(G293,G44,G103);
  or OR2_25(G294,G45,G104);
  or OR2_26(G295,G46,G105);
  or OR4_0(G300,G50,G49,G48,G47);
  or OR2_27(G306,G47,G324);
  or OR2_28(G307,G719,G305);
  or OR2_29(G312,G48,G324);
  or OR2_30(G313,G47,G311);
  or OR2_31(G318,G49,G324);
  or OR2_32(G319,G48,G317);
  or OR2_33(G324,G377,G348);
  or OR2_34(G325,G50,G324);
  or OR2_35(G326,G49,G323);
  or OR2_36(G333,G300,G714);
  or OR2_37(G334,G301,G714);
  or OR2_38(G337,G224,G355);
  or OR2_39(G338,G183,G336);
  or OR2_40(G340,G38,G355);
  or OR2_41(G341,G185,G339);
  or OR2_42(G344,G229,G348);
  or OR2_43(G345,G414,G343);
  or OR2_44(G349,G62,G348);
  or OR2_45(G350,G59,G347);
  or OR2_46(G352,G346,G645);
  or OR2_47(G353,G35,G351);
  or OR2_48(G355,G457,G645);
  or OR2_49(G356,G225,G355);
  or OR2_50(G357,G184,G354);
  or OR2_51(G372,G712,G358);
  or OR2_52(G391,G712,G377);
  or OR2_53(G404,G712,G413);
  or OR2_54(G423,G712,G432);
  or OR2_55(G434,G342,G645);
  or OR2_56(G439,G435,G63);
  or OR2_57(G448,G615,G65);
  or OR2_58(G456,G83,G524);
  or OR2_59(G492,G71,G500);
  or OR2_60(G493,G488,G491);
  or OR2_61(G496,G72,G500);
  or OR2_62(G497,G489,G495);
  or OR2_63(G500,G654,G712);
  or OR2_64(G501,G73,G500);
  or OR2_65(G502,G490,G499);
  or OR2_66(G505,G723,G511);
  or OR2_67(G506,G720,G504);
  or OR2_68(G508,G724,G511);
  or OR2_69(G509,G721,G507);
  or OR2_70(G512,G725,G511);
  or OR2_71(G513,G722,G510);
  or OR2_72(G518,G71,G67);
  or OR2_73(G519,G72,G68);
  or OR2_74(G520,G73,G69);
  or OR2_75(G521,G487,G70);
  or OR2_76(G522,G348,G228);
  or OR2_77(G523,G348,G414);
  or OR2_78(G524,G554,G555);
  or OR2_79(G563,G79,G610);
  or OR2_80(G564,G715,G562);
  or OR2_81(G567,G80,G610);
  or OR2_82(G568,G716,G566);
  or OR2_83(G571,G81,G610);
  or OR2_84(G572,G717,G570);
  or OR2_85(G575,G82,G610);
  or OR2_86(G576,G718,G574);
  or OR2_87(G583,G79,G74);
  or OR2_88(G584,G80,G75);
  or OR2_89(G585,G81,G76);
  or OR2_90(G586,G82,G77);
  or OR2_91(G587,G561,G78);
  or OR2_92(G591,G592,G604);
  or OR2_93(G594,G83,G593);
  or OR2_94(G602,G85,G601);
  or OR2_95(G603,G600,G84);
  or OR2_96(G606,G84,G610);
  or OR2_97(G607,G696,G605);
  or OR2_98(G610,G655,G712);
  or OR2_99(G611,G85,G610);
  or OR2_100(G612,G678,G609);
  or OR2_101(G618,G457,G645);
  or OR2_102(G619,G715,G617);
  or OR2_103(G621,G614,G645);
  or OR2_104(G622,G717,G620);
  or OR2_105(G624,G476,G645);
  or OR2_106(G625,G716,G623);
  or OR2_107(G627,G476,G645);
  or OR2_108(G628,G718,G626);
  or OR2_109(G630,G96,G645);
  or OR2_110(G631,G720,G629);
  or OR2_111(G633,G97,G645);
  or OR2_112(G634,G721,G632);
  or OR2_113(G636,G98,G645);
  or OR2_114(G637,G722,G635);
  or OR2_115(G639,G99,G645);
  or OR2_116(G640,G723,G638);
  or OR2_117(G642,G100,G645);
  or OR2_118(G643,G724,G641);
  or OR2_119(G646,G456,G645);
  or OR2_120(G647,G725,G644);
  or OR2_121(G666,G87,G678);
  or OR2_122(G667,G661,G665);
  or OR2_123(G672,G88,G678);
  or OR2_124(G673,G87,G671);
  or OR2_125(G679,G89,G678);
  or OR2_126(G680,G88,G677);
  or OR2_127(G682,G681,G699);
  or OR2_128(G684,G645,G696);
  or OR2_129(G687,G686,G699);
  or OR2_130(G690,G348,G696);
  or OR2_131(G691,G645,G689);
  or OR2_132(G693,G692,G699);
  or OR2_133(G697,G180,G696);
  or OR2_134(G698,G348,G695);
  or OR2_135(G699,G658,G712);
  nand NAND2_0(G96,G74,G596);
  nand NAND2_1(G97,G75,G596);
  nand NAND2_2(G98,G76,G596);
  nand NAND2_3(G99,G77,G596);
  nand NAND2_4(G100,G78,G596);
  nand NAND2_5(G106,G107,G110);
  nand NAND2_6(G107,G700,G111);
  nand NAND2_7(G122,G123,G126);
  nand NAND2_8(G123,G135,G127);
  nand NAND2_9(G137,G138,G141);
  nand NAND2_10(G138,G177,G142);
  nand NAND2_11(G163,G164,G167);
  nand NAND2_12(G164,G172,G178);
  nand NAND2_13(G186,G187,G190);
  nand NAND2_14(G187,G522,G191);
  nand NAND2_15(G205,G206,G209);
  nand NAND2_16(G206,G183,G210);
  nand NAND2_17(G211,G212,G215);
  nand NAND2_18(G212,G222,G216);
  nand NAND2_19(G230,G234,G235);
  nand NAND2_20(G231,G435,G648);
  nand NAND3_0(G232,G296,G298,G435);
  nand NAND3_1(G233,G700,G232,G231);
  nand NAND2_21(G234,G649,G436);
  nand NAND2_22(G266,G286,G291);
  nand NAND2_23(G267,G287,G292);
  nand NAND2_24(G268,G288,G293);
  nand NAND2_25(G269,G284,G294);
  nand NAND2_26(G270,G285,G295);
  nand NAND2_27(G284,G45,G104);
  nand NAND2_28(G285,G46,G105);
  nand NAND2_29(G286,G42,G101);
  nand NAND2_30(G287,G43,G102);
  nand NAND2_31(G288,G44,G103);
  nand NAND2_32(G297,G289,G290);
  nand NAND2_33(G298,G297,G700);
  nand NAND4_0(G301,G50,G49,G48,G47);
  nand NAND2_34(G331,G333,G22);
  nand NAND2_35(G332,G334,G331);
  nand NAND2_36(G476,G486,G616);
  nand NAND2_37(G482,G514,G518);
  nand NAND2_38(G483,G515,G519);
  nand NAND2_39(G484,G516,G520);
  nand NAND2_40(G485,G517,G521);
  nand NAND2_41(G514,G71,G67);
  nand NAND2_42(G515,G72,G68);
  nand NAND2_43(G516,G73,G69);
  nand NAND2_44(G517,G487,G70);
  nand NAND3_2(G554,G556,G557,G558);
  nand NAND2_45(G555,G559,G560);
  nand NAND2_46(G556,G578,G583);
  nand NAND2_47(G557,G579,G584);
  nand NAND2_48(G558,G580,G585);
  nand NAND2_49(G559,G581,G586);
  nand NAND2_50(G560,G582,G587);
  nand NAND2_51(G578,G79,G74);
  nand NAND2_52(G579,G80,G75);
  nand NAND2_53(G580,G81,G76);
  nand NAND2_54(G581,G82,G77);
  nand NAND2_55(G582,G561,G78);
  nand NAND2_56(G597,G602,G603);
  nand NAND2_57(G598,G435,G83);
  nand NAND4_1(G616,G482,G483,G484,G485);
  nand NAND2_58(G700,G282,G283);
  nor NOR2_0(G114,G115,G116);
  nor NOR2_1(G115,G133,G25);
  nor NOR2_2(G119,G120,G121);
  nor NOR2_3(G120,G134,G26);
  nor NOR2_4(G130,G131,G132);
  nor NOR2_5(G131,G136,G28);
  nor NOR2_6(G145,G146,G147);
  nor NOR2_7(G146,G168,G30);
  nor NOR2_8(G150,G151,G152);
  nor NOR2_9(G151,G169,G31);
  nor NOR2_10(G155,G156,G157);
  nor NOR2_11(G156,G170,G32);
  nor NOR2_12(G160,G161,G162);
  nor NOR2_13(G161,G171,G33);
  nor NOR2_14(G194,G195,G196);
  nor NOR2_15(G195,G202,G37);
  nor NOR2_16(G199,G200,G201);
  nor NOR2_17(G200,G203,G38);
  nor NOR2_18(G219,G220,G221);
  nor NOR2_19(G220,G223,G41);
  nor NOR2_20(G238,G239,G240);
  nor NOR2_21(G239,G299,G42);
  nor NOR2_22(G243,G244,G245);
  nor NOR2_23(G244,G262,G43);
  nor NOR2_24(G248,G249,G250);
  nor NOR2_25(G249,G263,G44);
  nor NOR2_26(G253,G254,G255);
  nor NOR2_27(G254,G264,G45);
  nor NOR2_28(G256,G257,G261);
  nor NOR2_29(G257,G265,G46);
  nor NOR3_0(G289,G270,G269,G268);
  nor NOR2_30(G290,G267,G266);
  nor NOR2_31(G299,G301,G328);
  nor NOR2_32(G327,G330,G23);
  nor NOR2_33(G328,G329,G327);
  nor NOR2_34(G361,G362,G363);
  nor NOR2_35(G362,G523,G51);
  nor NOR2_36(G366,G367,G368);
  nor NOR2_37(G367,G375,G52);
  nor NOR2_38(G369,G370,G374);
  nor NOR2_39(G370,G376,G53);
  nor NOR2_40(G380,G381,G382);
  nor NOR2_41(G381,G183,G54);
  nor NOR2_42(G385,G386,G387);
  nor NOR2_43(G386,G394,G55);
  nor NOR2_44(G388,G389,G393);
  nor NOR2_45(G389,G395,G56);
  nor NOR2_46(G398,G399,G400);
  nor NOR2_47(G399,G335,G57);
  nor NOR2_48(G401,G402,G406);
  nor NOR2_49(G402,G412,G58);
  nor NOR2_50(G409,G410,G411);
  nor NOR2_51(G410,G413,G59);
  nor NOR2_52(G417,G418,G419);
  nor NOR2_53(G418,G358,G60);
  nor NOR2_54(G420,G421,G425);
  nor NOR2_55(G421,G431,G61);
  nor NOR2_56(G428,G429,G430);
  nor NOR2_57(G429,G432,G62);
  nor NOR2_58(G442,G443,G444);
  nor NOR2_59(G444,G615,G64);
  nor NOR2_60(G452,G453,G454);
  nor NOR2_61(G454,G615,G66);
  nor NOR2_62(G460,G461,G462);
  nor NOR2_63(G461,G434,G67);
  nor NOR2_64(G465,G466,G467);
  nor NOR2_65(G466,G479,G68);
  nor NOR2_66(G470,G471,G472);
  nor NOR2_67(G471,G480,G69);
  nor NOR2_68(G473,G474,G478);
  nor NOR2_69(G474,G481,G70);
  nor NOR3_1(G487,G71,G72,G73);
  nor NOR2_70(G527,G528,G529);
  nor NOR2_71(G528,G604,G74);
  nor NOR2_72(G532,G533,G534);
  nor NOR2_73(G533,G550,G75);
  nor NOR2_74(G537,G538,G539);
  nor NOR2_75(G538,G551,G76);
  nor NOR2_76(G542,G543,G544);
  nor NOR2_77(G543,G552,G77);
  nor NOR2_78(G545,G546,G549);
  nor NOR2_79(G546,G553,G78);
  nor NOR4_0(G561,G79,G80,G81,G82);
  nor NOR2_80(G593,G435,G524);
  nor NOR2_81(G599,G598,G597);
  nor NOR2_82(G660,G658,G86);
  nor NOR2_83(G681,G683,G660);
  nor NOR2_84(G686,G688,G660);
  nor NOR2_85(G692,G694,G660);
  nor NOR4_1(G703,G650,G651,G652,G653);
  nor NOR2_86(G708,G709,G710);
  nor NOR2_87(G710,G678,G94);

endmodule
