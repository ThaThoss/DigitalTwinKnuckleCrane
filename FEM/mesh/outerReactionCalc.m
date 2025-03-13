syms Fax Fay Fbx Fby mgx mgy FLx FLy rabx raby racmx racmy rbcmx rbcmy raLx raLy rbLx rbLy rbax rbay max may MomZ real


%sum of forces, sum of moments a, sum of moments b


Fa = [Fax;Fay;0];
Fb = [Fbx;Fby;0];

mg = [mgx;mgy;0];
ma = [max;may;0];
FL = [FLx;FLy;0];

racm = [racmx ;racmy;0];
raL = [raLx ;raLy;0];
rab = [rabx ;raby;0];

skewRaL = skewThis(raL);
skewRab = skewThis(rab);
skewRacm = skewThis(racm);


rabFb = skewRab*Fb;
rabFb3 = rabFb(3);


%Moments around A
Ma = skewRacm*mg + skewRaL*FL - skewRacm*ma - MomZ;
ma3 = Ma(3);

FbxTemp = (FLy*raLx - FLx*raLy - MomZ- mgx*racmy + mgy*racmx + Fby*rabx - may*racmx + max*racmy)/raby;

%Moments around B
rbcm = [rbcmx; rbcmy;0];
rbL = [rbLx ;rbLy;0];
rba = [rbax ;rbay;0];

skewRbL = skewThis(rbL);
skewRba = skewThis(rba);
skewRbcm = skewThis(rbcm);

rbaFa = skewRba*Fa;
rbaFa3 = rbaFa(3);
Mb = skewRbcm*mg + skewRbL*FL-skewRbcm*ma - MomZ;
Mb3 = Mb(3);

FaxTemp = (FLy*rbLx - FLx*rbLy - MomZ - mgx*rbcmy + mgy*rbcmx + Fay*rbax - may*rbcmx + max*rbcmy)/rbLy;

%Sum forces X

  Fayy = mgx + FaxTemp + FbxTemp + FLx - max;


 



Fayy2 = simplify(-((FLx - max + mgx - (MomZ + FLx*raLy - FLy*raLx - Fby*rabx + may*racmx - max*racmy + mgx*racmy - mgy*racmx)/raby)*rbLy - (MomZ + FLx*rbLy - FLy*rbLx + may*rbcmx - max*rbcmy + mgx*rbcmy - mgy*rbcmx))/rbax);
  


  %sum forces Y

Fbyy =  mgy + FLy + Fayy2 + Fby - may;

FbyFinal = (-(FLy - may + mgy + (MomZ*raby + MomZ*rbLy + FLx*raLy*rbLy - FLy*raLx*rbLy - FLy*raby*rbLx + max*raby*rbLy + may*racmx*rbLy - max*racmy*rbLy + may*raby*rbcmx - max*raby*rbcmy - mgx*raby*rbLy + mgx*racmy*rbLy - mgy*racmx*rbLy + mgx*raby*rbcmy - mgy*raby*rbcmx)/(raby*rbax)))/(1-rabx*rbLy/(raby*rbax));

FbyFinal = simplify(FbyFinal);

FbxFinal = (FLy*raLx - FLx*raLy - mgx*racmy + mgy*racmx + FbyFinal*rabx)/raby;
FbxFinal = simplify(FbxFinal);


FayFinal = simplify((MomZ*raby + MomZ*rbLy + FLx*raLy*rbLy - FLy*raLx*rbLy - FLy*raby*rbLx - FbyFinal*rabx*rbLy + max*raby*rbLy + may*racmx*rbLy - max*racmy*rbLy + may*raby*rbcmx - max*raby*rbcmy - mgx*raby*rbLy + mgx*racmy*rbLy - mgy*racmx*rbLy + mgx*raby*rbcmy - mgy*raby*rbcmx)/(raby*rbax));
FayFinal = simplify(FayFinal);

FaxFinal = (FLy*rbLx - FLx*rbLy - MomZ - mgx*rbcmy + mgy*rbcmx + FayFinal*rbax - may*rbcmx + max*rbcmy)/rbLy;
FaxFinal = simplify(FaxFinal);

