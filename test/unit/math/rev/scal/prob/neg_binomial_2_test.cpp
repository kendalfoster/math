#include <stan/math/rev/scal.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

struct TestValue {
  unsigned int n;
  double mu;
  double phi;
  double value;
  double grad_mu;
  double grad_phi;

  TestValue(unsigned int _n, double _mu, double _phi, double _value,
            double _grad_mu, double _grad_phi)
      : n(_n),
        mu(_mu),
        phi(_phi),
        value(_value),
        grad_mu(_grad_mu),
        grad_phi(_grad_phi) {}
};

// Test data generated in Mathematica (Wolfram Cloud) via
// nb2[n_,mu_,phi_]:= LogGamma[n + phi] - LogGamma[n + 1] - LogGamma[phi ]+
//   n * (Log[mu] - Log[mu + phi]) + phi * (Log[phi] - Log[mu + phi])
// nb2dmu[n_,mu_,phi_]= D[nb2[n, mu, phi],mu];
// nb2dphi[n_,mu_,phi_]= D[nb2[n, mu, phi],phi];
// out = OpenWrite["nb_test.txt"]
// mus= {256*10^-7,314*10^-3,15*10^-1,3,180,  1123,10586};
//  phis=  {4*10^-4,65*10^-3,442*10^-2,800, 15324};
// ns = {0,6,14,1525,10233};
//  WriteString[out, "std::array<TestValue, ",
//      Length[mus]*Length[phis]*Length[ns], "> testValues = {"];
//    Block[{$MaxPrecision = 80, $MinPrecision = 40}, {
//      For[i = 1, i <= Length[mus], i++, {
//        For[j = 1, j <= Length[phis], j++, {
//        For[k = 1, k <= Length[ns], k++, {
//          cmu = mus[[i]];
//          cphi = phis[[j]];
//     cn=ns[[k]];
//          val = N[nb2[cn,cmu,cphi]];
//     ddmu= N[nb2dmu[cn,cmu,cphi]];
//     ddphi= N[nb2dphi[cn,cmu,cphi]];
//          WriteString[out,"  TestValue(",CForm[cn],",",CForm[cmu],",",
//            CForm[cphi],",", CForm[val],","CForm[ddmu],",",CForm[ddphi],"),"]
//        }]
//      }]
//   }]
//    }];
//  WriteString[out,"};"];
//  Close[out];
//  FilePrint[%]
std::array<TestValue, 210> testValues = {
    TestValue(0, 0.0000256, 0.0004, -0.000024814156367780882,
              -0.9398496240601504, -0.0018850149796021398),
    TestValue(0, 0.0000256, 0.065, -0.000025594960092480967,
              -0.9996063088998794, -7.751668684718993e-8),
    TestValue(0, 0.0000256, 4.42, -0.000025599925864390194, -0.9999942081783417,
              -1.677258332222209e-11),
    TestValue(0, 0.0000256, 800, -0.00002559999998652529, -0.999999968000001,
              -8.881784197001252e-16),
    TestValue(0, 0.0000256, 15324, -0.000025599996959613236,
              -0.9999999983294179, 0.),
    TestValue(0, 0.0000256, 150000, -0.00002560005540885868,
              -0.9999999998293333, 0.),
    TestValue(0, 0.314, 0.0004, -0.002666782716971683, -0.001272264631043257,
              -5.66822905706025),
    TestValue(0, 0.314, 0.065, -0.11460468078714132, -0.17150395778364116,
              -0.9346528929704305),
    TestValue(0, 0.314, 4.42, -0.30334820230965986, -0.9336713138994508,
              -0.00230212890163628),
    TestValue(0, 0.314, 800, -0.31393839362010567, -0.9996076539958066,
              -7.698783210940974e-8),
    TestValue(0, 0.314, 15324, -0.31399678298770084, -0.9999795096863716,
              -2.099280749234822e-10),
    TestValue(0, 0.314, 150000, -0.3139996713308335, -0.9999979066710487,
              -2.190247982980509e-12),
    TestValue(0, 1.5, 0.0004, -0.003291911100032755, -0.00026659557451346307,
              -7.230044345656401),
    TestValue(0, 1.5, 0.065, -0.2067814991501101, -0.04153354632587859,
              -2.2227873794044952),
    TestValue(0, 1.5, 4.42, -1.2915096474038796, -0.7466216216216216,
              -0.03881837442792935),
    TestValue(0, 1.5, 800, -1.4985955053447242, -0.9981285090455396,
              -1.7534272203079126e-6),
    TestValue(0, 1.5, 15324, -1.499926590534038, -0.9999021239111285,
              -4.790177143831897e-9),
    TestValue(0, 1.5, 150000, -1.4999925000758196, -0.999990000099999,
              -4.999911595859885e-11),
    TestValue(0, 8, 0.0004, -0.003961415020514468, -0.00004999750012499375,
              -8.903587548786295),
    TestValue(0, 8, 0.065, -0.3133586118431387, -0.008059516429014259,
              -3.8289612370926864),
    TestValue(0, 8, 4.42, -4.566604241437509, -0.355877616747182,
              -0.3890459971629081),
    TestValue(0, 8, 800, -7.9602646825343015, -0.9900990099009901,
              -0.000049340754157967126),
    TestValue(0, 8, 15324, -7.99791249886043, -0.9994782154969998,
              -1.361769044194716e-7),
    TestValue(0, 8, 150000, -7.999786674428577, -0.9999466695109595,
              -1.4221228639144101e-9),
    TestValue(0, 180, 0.0004, -0.0052068020335865025, -2.2222172839615914e-6,
              -12.01700730618354),
    TestValue(0, 180, 0.065, -0.5152345838836883, -0.00036098075694887956,
              -6.927046886659846),
    TestValue(0, 180, 4.42, -16.491356225074536, -0.023967031775295522,
              -2.755043100796684),
    TestValue(0, 180, 800, -162.35267519735217, -0.8163265306122449,
              -0.01926737460893513),
    TestValue(0, 180, 15324, -178.95104102256036, -0.9883900928792569,
              -0.00006792118926490787),
    TestValue(0, 180, 150000, -179.8920863223863, -0.9988014382740711,
              -7.188495541043949e-7),
    TestValue(0, 1123, 0.0004, -0.005939122128713381, -3.5618865318302646e-7,
              -13.847805677972104),
    TestValue(0, 1123, 0.065, -0.6342170147837343, -0.000057877326779839104,
              -8.75724272015346),
    TestValue(0, 1123, 4.42, -24.493639593452325, -0.00392045555338738,
              -4.545467874886493),
    TestValue(0, 1123, 800, -701.6240143366822, -0.4160166406656266,
              -0.29304665858647905),
    TestValue(0, 1123, 15324, -1083.757151397245, -0.9317200705295798,
              -0.002442933450309326),
    TestValue(0, 1123, 150000, -1118.817101006986, -0.9925689670003904,
              -0.000027747673771116865),
    TestValue(0, 10586, 0.0004, -0.006836533480423993, -3.778575334268833e-8,
              -16.091333738845734),
    TestValue(0, 10586, 0.065, -0.7800430171085975, -6.1401474485561915e-6,
              -11.00066794181818),
    TestValue(0, 10586, 4.42, -34.3945190758502, -0.0004173583295091224,
              -6.781982760105572),
    TestValue(0, 10586, 800, -2124.4224654744803, -0.07026172492534692,
              -1.7257898067684474),
    TestValue(0, 10586, 15324, -8048.29915678056, -0.5914318795831726,
              -0.11664064731878732),
    TestValue(0, 10586, 150000, -10229.149577360897, -0.9340789358972762,
              -0.0022732664130149516),
    TestValue(7, 0.0000256, 0.0004, -29.445354199147125, 256989.1917293233,
              -13944.92090243279),
    TestValue(7, 0.0000256, 0.065, -59.40270770911646, 273328.85048350185,
              -89.90748568276743),
    TestValue(7, 0.0000256, 4.42, -79.18162818224879, 273434.9163045572,
              -0.5615296684557465),
    TestValue(7, 0.0000256, 800, -82.50943543843826, 273436.49125003227,
              -0.00003263555538968177),
    TestValue(7, 0.0000256, 15324, -82.5342442124603, 273436.4995432019,
              -8.94022633701752e-8),
    TestValue(7, 0.0000256, 150000, -82.53547440969385, 273436.4999533335,
              -9.33299659777731e-10),
    TestValue(7, 0.314, 0.0004, -9.780554584441889, 0.027090322685207696,
              2474.516543534505),
    TestValue(7, 0.314, 0.065, -5.954681567099272, 3.6518326807051746,
              -1.661885301087814),
    TestValue(7, 0.314, 4.42, -14.063434552944763, 19.880657339909963,
              -0.4587955394328642),
    TestValue(7, 0.314, 800, -16.924203575168434, 21.284639409604978,
              -0.000029279795684011845),
    TestValue(7, 0.314, 15324, -16.946467423587734, 21.292557330455672,
              -8.025298470215603e-8),
    TestValue(7, 0.314, 150000, -16.947571739258493, 21.29294905733322,
              -8.378080451620917e-10),
    TestValue(7, 1.5, 0.0004, -9.774134608119379, 0.0009775171065493646,
              2490.5539367351803),
    TestValue(7, 1.5, 0.065, -5.026800862430695, 0.15228966986155484,
              11.046793754083321),
    TestValue(7, 1.5, 4.42, -5.669765754393499, 2.7376126126126126,
              -0.1990792406277535),
    TestValue(7, 1.5, 800, -7.172434628346302, 3.6598045331669784,
              -0.000018013716757359077),
    TestValue(7, 1.5, 15324, -7.186147155348181, 3.6663077876741377,
              -4.948342535726624e-8),
    TestValue(7, 1.5, 150000, -7.1868281060611565, 3.666630000366663,
              -5.166445049553658e-10),
    TestValue(7, 8, 0.0004, -9.773287685468139, -6.249687515624219e-6,
              2492.6708598338487),
    TestValue(7, 8, 0.065, -4.893078153217412, -0.0010074395536267824,
              13.045515423749752),
    TestValue(7, 8, 4.42, -2.4138267066913563, -0.04448470209339775,
              0.06951848372348413),
    TestValue(7, 8, 800, -1.972808375834589, -0.12376237623762376,
              4.6570738199136485e-6),
    TestValue(7, 8, 15324, -1.9692663072788577, -0.12493477693712497,
              1.277112104958178e-8),
    TestValue(7, 8, 150000, -1.9690905691403486, -0.12499333368886993,
              1.333280152948646e-10),
    TestValue(7, 180, 0.0004, -9.774198636769189, -2.1357977229186406e-6,
              2490.3935075261693),
    TestValue(7, 180, 0.065, -5.040836257459913, -0.00034694261640086756,
              10.776502846558865),
    TestValue(7, 180, 4.42, -11.4293253460954, -0.023034980539589585,
              -1.770828372217259),
    TestValue(7, 180, 800, -135.92154531818449, -0.7845804988662132,
              -0.017692867587181027),
    TestValue(7, 180, 15324, -151.2058790199169, -0.949952700378397,
              -0.00006270718914258566),
    TestValue(7, 180, 150000, -152.07480469327567, -0.9599591601189683,
              -6.638499794320296e-7),
    TestValue(7, 1123, 0.0004, -9.77491789464706, -3.539684211507191e-7,
              2488.5953646554194),
    TestValue(7, 1123, 0.065, -5.157696519888815, -0.00005751655982751597,
              8.978948920929607),
    TestValue(7, 1123, 4.42, -19.28929343971758, -0.003896018163473122,
              -3.5295051749149056),
    TestValue(7, 1123, 800, -667.0958939480461, -0.41342348262051587,
              -0.287969440027692),
    TestValue(7, 1123, 15324, -1043.6096899099389, -0.9259123764122984,
              -0.0024118325958113473),
    TestValue(7, 1123, 150000, -1078.228021151623, -0.9863819832345821,
              -0.000027401825537509694),
    TestValue(7, 10586, 0.0004, -9.77581307717804, -3.7760767486519914e-8,
              2486.357408645293),
    TestValue(7, 10586, 0.065, -5.303160350365611, -6.136087271705644e-6,
              6.741095395500565),
    TestValue(7, 10586, 4.42, -29.16559791545973, -0.00041708235101804323,
              -5.760472168602957),
    TestValue(7, 10586, 800, -2086.6391306495434, -0.0702152643099608,
              -1.7176872326969326),
    TestValue(7, 10586, 15324, -7995.6283957432015, -0.5910407948337788,
              -0.11645410288972613),
    TestValue(7, 10586, 150000, -10173.280945470673, -0.9334612755391352,
              -0.0022701910299964823),
    TestValue(14, 0.0000256, 0.0004, -49.814562438358635, 513979.32330827066,
              -30391.559221530802),
    TestValue(14, 0.0000256, 0.065, -114.92817109028836, 546658.7005733127,
              -196.83239209326592),
    TestValue(14, 0.0000256, 4.42, -161.64931287705224, 546870.8326033225,
              -1.650128690902025),
    TestValue(14, 0.0000256, 800, -173.09898662779824, 546873.9825000325,
              -0.00014060727924558591),
    TestValue(14, 0.0000256, 15324, -173.20616504262574, 546873.9990864021,
              -3.872936886750722e-7),
    TestValue(14, 0.0000256, 150000, -173.21149502739468, 546873.9999066668,
              -4.044185430984726e-9),
    TestValue(14, 0.314, 0.0004, -10.482321240387563, 0.05545291000145865,
              2452.982014445862),
    TestValue(14, 0.314, 0.065, -7.917539720426937, 7.4751693191939905,
              -19.40653851445296),
    TestValue(14, 0.314, 4.42, -31.109603016060397, 40.69498599371937,
              -1.4423583039713965),
    TestValue(14, 0.314, 800, -41.61461010763844, 43.568886473205765,
              -0.00013381877200302483),
    TestValue(14, 0.314, 15324, -41.71664028188991, 43.58509417059771,
              -3.687852014877535e-7),
    TestValue(14, 0.314, 150000, -41.72171561524851, 43.58589602133749,
              -3.851015506484146e-9),
    TestValue(14, 1.5, 0.0004, -10.468856159359483, 0.002221629787612192,
              2486.6186161358087),
    TestValue(14, 1.5, 0.065, -5.969601492726813, 0.34611288604898827,
              7.298954082323366),
    TestValue(14, 1.5, 4.42, -13.334103973863655, 6.221846846846847,
              -0.8864094608348823),
    TestValue(14, 1.5, 800, -20.926415102269573, 8.317737575379496,
              -0.00010961017476240897),
    TestValue(14, 1.5, 15324, -21.010069937864472, 8.332517699259403,
              -3.0266583905813604e-7),
    TestValue(14, 1.5, 150000, -21.014235520108855, 8.333250000833324,
              -3.160877781738236e-9),
    TestValue(14, 8, 0.0004, -10.466492810136518, 0.00003749812509374531,
              2492.526005536275),
    TestValue(14, 8, 0.065, -5.595578961607217, 0.006044637321760695,
              12.902571279344416),
    TestValue(14, 8, 4.42, -3.5471312844257383, 0.2669082125603865,
              0.0010136106025722125),
    TestValue(14, 8, 800, -4.065493420056555, 0.7425742574257426,
              -0.00001668126667020431),
    TestValue(14, 8, 15324, -4.078322333399456, 0.7496086616227498,
              -4.677001541608661e-8),
    TestValue(14, 8, 150000, -4.0789662719144815, 0.7499600021332196,
              -4.888125459956427e-10),
    TestValue(14, 180, 0.0004, -10.467069325725546, -2.04937816187569e-6,
              2491.084720678314),
    TestValue(14, 180, 0.065, -5.689219198051667, -0.0003329044758528556,
              11.4626317745298),
    TestValue(14, 180, 4.42, -9.653376579596795, -0.022102929303883648,
              -1.313682997645138),
    TestValue(14, 180, 800, -117.57055678993851, -0.7528344671201814,
              -0.016193696733894036),
    TestValue(14, 180, 15324, -131.5984192349756, -0.911515307877537,
              -0.000057701678183974536),
    TestValue(14, 180, 150000, -132.39809487222743, -0.9211168819638655,
              -6.110279944238073e-7),
    TestValue(14, 1123, 0.0004, -10.467775521386166, -3.517481891184117e-7,
              2489.3192333086026),
    TestValue(14, 1123, 0.065, -5.803957292009425, -0.000057155792875192846,
              9.697719756764902),
    TestValue(14, 1123, 4.42, -17.37102939846337, -0.003871580773558864,
              -3.040611828950623),
    TestValue(14, 1123, 800, -640.6479149103316, -0.4108303245754051,
              -0.2829675576373729),
    TestValue(14, 1123, 15324, -1011.5999306403351, -0.920104682295017,
              -0.00238094023047708),
    TestValue(14, 1123, 150000, -1045.7795131043224, -0.9801949994687738,
              -0.00002705815489179031),
    TestValue(14, 10586, 0.0004, -10.468668475096411, -3.7735781630351504e-8,
              2487.0868493492235),
    TestValue(14, 10586, 0.065, -5.949058950638154, -6.132027094855097e-6,
              7.465437927571536),
    TestValue(14, 10586, 4.42, -27.2227588675498, -0.0004168063725269641,
              -5.266030931107645),
    TestValue(14, 10586, 800, -2056.9359371755286, -0.07016880369457469,
              -1.7096599947938858),
    TestValue(14, 10586, 15324, -7951.095336923546, -0.590649710084385,
              -0.11626776694982688),
    TestValue(14, 10586, 150000, -10125.552885388513, -0.9328436151809941,
              -0.002267117824567677),
    TestValue(1525, 0.0000256, 0.0004, -4301.784727467015, 5.5987134868421055e7,
              -3.580668787634408e6),
    TestValue(1525, 0.0000256, 0.065, -11965.467128123484, 5.954685919853104e7,
              -23429.11268911477),
    TestValue(1525, 0.0000256, 4.42, -18367.34289180058, 5.956996647937967e7,
              -339.0570825343945),
    TestValue(1525, 0.0000256, 800, -24826.22813563853, 5.957030959375009e7,
              -0.8389762876176414),
    TestValue(1525, 0.0000256, 15324, -25707.717028801628, 5.957031140048291e7,
              -0.004643062913324059),
    TestValue(1525, 0.0000256, 150000, -25773.43154710444, 5.957031148983333e7,
              -0.00005129937594539058),
    TestValue(1525, 0.314, 0.0004, -17.094738795518424, 6.1777199721236284,
              -2348.2711549792057),
    TestValue(1525, 0.314, 0.065, -296.58556828610017, 832.7696922844226,
              -4001.4921811261693),
    TestValue(1525, 0.314, 4.42, -4115.10160678526, 4533.61649969458,
              -316.1764856190919),
    TestValue(1525, 0.314, 800, -10469.94267675256, 4853.782788344746,
              -0.8382285160349765),
    TestValue(1525, 0.314, 15324, -11350.864477542971, 4855.588403521258,
              -0.004641024152967077),
    TestValue(1525, 0.314, 150000, -11416.550945303003, 4855.677733537116,
              -0.00005127809769334135),
    TestValue(1525, 1.5, 0.0004, -15.560537541616213, 0.270772238514174,
              1484.280307752038),
    TestValue(1525, 1.5, 0.065, -74.45092758687497, 42.184238551650694,
              -953.4745083343279),
    TestValue(1525, 1.5, 4.42, -2072.1910365790172, 758.3186936936937,
              -251.67662613527665),
    TestValue(1525, 1.5, 800, -8088.548799680884, 1013.7658556872531,
              -0.8354105724129193),
    TestValue(1525, 1.5, 15324, -8967.33164084002, 1015.5672571857361,
              -0.004633327525485953),
    TestValue(1525, 1.5, 150000, -9032.912208922884, 1015.6565101015657,
              -0.00005119776202988646),
    TestValue(1525, 8, 0.0004, -15.230842685268726, 0.00948077596120194,
              2308.3869231549475),
    TestValue(1525, 8, 0.065, -22.20647206997819, 1.528285802851829,
              -169.72844230404513),
    TestValue(1525, 8, 4.42, -652.6338020806143, 67.48329307568439,
              -117.2113317142287),
    TestValue(1525, 8, 800, -5554.513964530293, 187.74752475247524,
              -0.8201519269955586),
    TestValue(1525, 8, 15324, -6421.662226247852, 189.52605661361858,
              -0.004591272777291877),
    TestValue(1525, 8, 150000, -6486.664023141206, 189.61488720601568,
              -0.00005075860649661479),
    TestValue(1525, 180, 0.0004, -15.159228863591125, 0.000016604901371824113,
              2487.4167692289852),
    TestValue(1525, 180, 0.065, -10.618383986015033, 0.0026973284338680167,
              7.7929628140754765),
    TestValue(1525, 180, 4.42, -30.756932642065294, 0.1790869874320693,
              -5.0606677133089075),
    TestValue(1525, 180, 800, -1255.1060610431614, 6.099773242630386,
              -0.5081161722061651),
    TestValue(1525, 180, 15324, -1861.5172673616398, 7.38547041623667,
              -0.0035556000115217756),
    TestValue(1525, 180, 150000, -1912.1930479564326, 7.463266302659031,
              -0.00003983284968711587),
    TestValue(1525, 1123, 0.0004, -15.157115486355906, 1.2750475385536657e-7,
              2492.70020501204),
    TestValue(1525, 1123, 0.065, -10.275036857130544, 0.000020718330690556827,
              13.074039765317043),
    TestValue(1525, 1123, 4.42, -7.7548168672583415, 0.0014034043922188128,
              0.0654298516080507),
    TestValue(1525, 1123, 800, -30.39018204828426, 0.14892136201921807,
              -0.018804728472968435),
    TestValue(1525, 1123, 15324, -64.3938471491183, 0.33352757644959136,
              -0.00029097614079454104),
    TestValue(1525, 1123, 150000, -68.69059192176792, 0.3553096391221344,
              -3.4982159551333325e-6),
    TestValue(1525, 10586, 0.0004, -15.157527333190956, -3.234240610599839e-8,
              2491.6705880068075),
    TestValue(1525, 10586, 0.065, -10.34196113541293, -5.255608920401252e-6,
              12.044448366424728),
    TestValue(1525, 10586, 4.42, -12.301855613945008, -0.0003572344439525938,
              -0.9624372357797295),
    TestValue(1525, 10586, 800, -744.0169167062886, -0.060139948001942986,
              -0.7924525685250288),
    TestValue(1525, 10586, 15324, -4300.645593413912, -0.506231273465249,
              -0.08062419697917278),
    TestValue(1525, 10586, 150000, -5850.263517824056, -0.799517215016552,
              -0.0016543683056511327),
    TestValue(10233, 0.0000256, 0.0004, -28781.07085289515,
              3.7568285855263156e8, -2.4041193199521683e7),
    TestValue(10233, 0.0000256, 0.065, -80237.4790550341, 3.995691927102557e8,
              -157343.697098361),
    TestValue(10233, 0.0000256, 4.42, -123371.16336416776, 3.997242463550437e8,
              -2307.279979293951),
    TestValue(10233, 0.0000256, 800, -173733.65594636812, 3.997265487087504e8,
              -10.166635446653597),
    TestValue(10233, 0.0000256, 15324, -189611.39394380094,
              3.9972656083222395e8, -0.15627194690990365),
    TestValue(10233, 0.0000256, 150000, -192108.97687250923, 3.9972656143178e8,
              -0.0022260752765799197),
    TestValue(10233, 0.314, 0.0004, -30.083534640958533, 41.460778593539814,
              -30043.568270589793),
    TestValue(10233, 0.314, 0.065, -1936.716591758479, 5589.00047056451,
              -26975.841613138524),
    TestValue(10233, 0.314, 4.42, -27734.55588693275, 30426.641345033488,
              -2153.7342775414127),
    TestValue(10233, 0.314, 800, -77398.83453431107, 32575.386135464123,
              -10.16161733713528),
    TestValue(10233, 0.314, 15324, -93272.76691554434, 32587.50423265796,
              -0.15625826529496223),
    TestValue(10233, 0.314, 150000, -95770.16160401958, 32588.103756758428,
              -0.0022259324835118832),
    TestValue(10233, 1.5, 0.0004, -19.785223133323598, 1.8184484137563317,
              -4317.601447204935),
    TestValue(10233, 1.5, 0.065, -445.630392408013, 283.3003194888179,
              -6515.787894589817),
    TestValue(10233, 1.5, 4.42, -14020.648493631525, 5092.706081081081,
              -1720.7211329093311),
    TestValue(10233, 1.5, 800, -61412.52664040723, 6808.234560199626,
              -10.142698900729565),
    TestValue(10233, 1.5, 15324, -77272.09898456372, 6820.332387197807,
              -0.15620659350885369),
    TestValue(10233, 1.5, 150000, -79768.7827089832, 6820.931790682093,
              -0.0022253931450428865),
    TestValue(10233, 8, 0.0004, -17.56909362175793, 0.06390305484725764,
              1221.8452476349516),
    TestValue(10233, 8, 0.065, -94.45295843966596, 10.30106943583385,
              -1247.5517925303861),
    TestValue(10233, 8, 4.42, -4476.485408813824, 454.85607890499193,
              -816.4371067130297),
    TestValue(10233, 8, 800, -44371.840290708235, 1265.470297029703,
              -10.040039157123783),
    TestValue(10233, 8, 15324, -60153.13532501529, 1277.4580941821027,
              -0.15592364901449507),
    TestValue(10233, 8, 150000, -62645.92507437721, 1278.056836968695,
              -0.002222438504382751),
    TestValue(10233, 180, 0.0004, -17.08144177432405, 0.00012411083530925487,
              2440.94300115826),
    TestValue(10233, 180, 0.065, -15.542242814932251, 0.020160775275594924,
              -38.6634853761835),
    TestValue(10233, 180, 4.42, -235.49737914944126, 1.3385587246502548,
              -50.37753458197702),
    TestValue(10233, 180, 800, -14640.50246307096, 45.59183673469388,
              -7.836489965276398),
    TestValue(10233, 180, 15324, -28577.644697168085, 55.2015866873065,
              -0.14858705144088802),
    TestValue(10233, 180, 150000, -30968.941707557387, 55.78306032760687,
              -0.0021450282568338253),
    TestValue(10233, 1123, 0.0004, -17.063078998820856, 2.8894734020457448e-6,
              2486.849880233364),
    TestValue(10233, 1123, 0.065, -12.558918107941281, 0.0004695124193805291,
              7.224124958319663),
    TestValue(10233, 1123, 4.42, -35.45506157865748, 0.031803517445555686,
              -5.756960605210265),
    TestValue(10233, 1123, 800, -3343.1303902972795, 3.3748099701370067,
              -2.9898053694742615),
    TestValue(10233, 1123, 15324, -11352.060718035325, 7.558299058347705,
              -0.11311918072723515),
    TestValue(10233, 1123, 150000, -13237.322258064814, 8.051917443787673,
              -0.0017468778907598903),
    TestValue(10233, 10586, 0.0004, -17.060718192660715, -1.2600010324928188e-9,
              2492.7518943576556),
    TestValue(10233, 10586, 0.065, -12.175300607224926, -2.047489183204549e-7,
              13.125723676609066),
    TestValue(10233, 10586, 4.42, -9.430792045634007, -0.00001391720105013416,
              0.11674937200192481),
    TestValue(10233, 10586, 800, -7.2703661972227565, -0.0023429424616141564,
              0.00008904830662359586),
    TestValue(10233, 10586, 15324, -9.327824035135563, -0.019721845219427537,
              -0.00008059480749977865),
    TestValue(10233, 10586, 150000, -11.132452601363184, -0.031147729489111892,
              -2.2067081495436014e-6),
};

TEST(ProbDistributionsNegBinomial, derivativesPrecomputed) {
  using stan::math::digamma;
  using stan::math::is_nan;
  using stan::math::neg_binomial_2_log;
  using stan::math::value_of;
  using stan::math::var;

  for (auto iter = testValues.begin(); iter != testValues.end(); ++iter) {
    TestValue t = *iter;
    unsigned int n = t.n;
    var mu(t.mu);
    var phi(t.phi);
    var val = neg_binomial_2_log(n, mu, phi);

    std::vector<var> x;
    x.push_back(mu);
    x.push_back(phi);

    std::vector<double> gradients;
    val.grad(x, gradients);

    for (int i = 0; i < 2; ++i) {
      EXPECT_FALSE(is_nan(gradients[i]));
    }

    auto tolerance = [](double x) { return std::max(fabs(x * 1e-8), 1e-14); };

    EXPECT_NEAR(value_of(val), t.value, tolerance(t.value))
        << "value n = " << n << ", mu = " << t.mu << ", phi = " << t.phi;
    EXPECT_NEAR(gradients[0], t.grad_mu, tolerance(t.grad_mu))
        << "grad_mu n = " << n << ", mu = " << t.mu << ", phi = " << t.phi;
    EXPECT_NEAR(gradients[1], t.grad_phi, tolerance(t.grad_phi))
        << "grad_phi n = " << n << ", mu = " << t.mu << ", phi = " << t.phi
        << ", digamma = " << digamma(t.phi) << ", " << digamma(t.n + t.phi);
  }
}

TEST(ProbDistributionsNegBinomial, derivativesFiniteDiffs) {
  using stan::math::is_nan;
  using stan::math::neg_binomial_2_log;
  using stan::math::var;

  std::array<unsigned int, 3> n_to_test = {7, 100, 835};
  std::array<double, 2> mu_to_test = {8, 24};
  // std::array<unsigned int, 5> n_to_test = {0, 7, 100, 835};
  // std::array<double, 6> mu_to_test = {0.8, 8, 24, 271};
  double phi_cutoff = stan::math::internal::neg_binomial_2_phi_cutoff;
  for (auto mu_iter = mu_to_test.begin(); mu_iter != mu_to_test.end();
       ++mu_iter) {
    for (auto n_iter = n_to_test.begin(); n_iter != n_to_test.end(); ++n_iter) {
      double mu_dbl = *mu_iter;
      unsigned int n = *n_iter;
      double phi_dbl = 1.5;

      for (int k = 0; k < 20; ++k) {
        var mu(mu_dbl);
        var phi(phi_dbl);
        var val = neg_binomial_2_log(n, mu, phi);

        std::vector<var> x;
        x.push_back(mu);
        x.push_back(phi);

        std::vector<double> gradients;
        val.grad(x, gradients);

        EXPECT_TRUE(value_of(val) < 0)
            << "for n = " << n << ", mu = " << mu_dbl << ", phi = " << phi_dbl;

        for (int i = 0; i < 2; ++i) {
          EXPECT_FALSE(is_nan(gradients[i]));
        }

        std::vector<double> finite_diffs;
        double eps = 1e-10;
        double inv2e = 0.5 / eps;
        double dmu = neg_binomial_2_log(n, mu_dbl + eps, phi_dbl)
                     - neg_binomial_2_log(n, mu_dbl - eps, phi_dbl);
        double dphi = neg_binomial_2_log(n, mu_dbl, phi_dbl + eps)
                      - neg_binomial_2_log(n, mu_dbl, phi_dbl - eps);
        finite_diffs.push_back(dmu * inv2e);
        finite_diffs.push_back(dphi * inv2e);

        EXPECT_NEAR(gradients[0], finite_diffs[0],
                    std::max(1.0, gradients[0] * 1e-4))
            << "grad_mu, n = " << n << ", mu = " << mu_dbl
            << "  +/- epsilon, phi = " << phi_dbl
            << " +/- epsilon, dmu = " << dmu;
        EXPECT_NEAR(gradients[1], finite_diffs[1],
                    std::max(1.0, gradients[1] * 1e-4))
            << "grad_phi, n = " << n << ", mu = " << mu_dbl
            << "  +/- epsilon, phi = " << phi_dbl
            << " +/- epsilon, dphi = " << dphi;

        phi_dbl *= 10;
      }
    }
  }
}

TEST(ProbDistributionsNegativeBinomial2, proptoAtPoissonCutoff) {
  using stan::math::internal::neg_binomial_2_phi_cutoff;
  using stan::math::neg_binomial_2_lpmf;
  using stan::math::var;

  var mu_var(10);
  int y = 11;
  var value_before_cutoff = neg_binomial_2_lpmf<true, int, var, double>(
      y, mu_var, neg_binomial_2_phi_cutoff - 1e-8);
  var value_after_cutoff = neg_binomial_2_lpmf<true, int, var, double>(
      y, mu_var, neg_binomial_2_phi_cutoff + 1e-8);

  EXPECT_NEAR(value_of(value_before_cutoff), value_of(value_after_cutoff), 1);
}

// TODO(martinmodrak) test continuity of derivatives at cutoff
