clc
clear all

filename = "mainBoomMesh.dat";
fileID = fopen(filename,'r');
tline = fgetl(fileID)
tline = fgetl(fileID)
tline = fgetl(fileID)
tline = fgetl(fileID)
tline = fgetl(fileID)
nElements = 2220;
nNodes = 2397;
AllElements=fscanf(fileID,"%d %d %d %d %d %d\n",[6,nElements])';

tline = fgetl(fileID)
Nodes=fscanf(fileID,"%d %f %f\n",[3,nNodes])';
% Nodes(89,2:3) = [4.477,0.45];
% Nodes(1842,2:3) = [4.441,0.415];
% Nodes(501,2:3) = [4.455,0.425];
% Nodes(1841,2:3) = [4.468,0.435];


BoundaryEdges = getBoundaryEdgesMain();

elementProblemNr = [728]+1; %[568,588,589,591,604,607,620,636]+1;
nNodeElement=4
tline = fgetl(fileID)



fclose(fileID);
Elements= [AllElements(:,1:5),zeros(nElements,1)];


f1 = figure(1);
clf(f1,'reset');
grid on
hold on
numBound1 = 0;
numBound2 = 0;
numBound3 = 0;
numBound4 = 0;
numBound5 = 0;
forceNodeHoleRight=[];
forceNodeHoleRightCounter =1;
lockedNodesHole=[];
lockedHoleCounter = 1;
forceEdgeNodes = [];
forceEdgeCouner = 1;
for j=1:size(BoundaryEdges,1)
    
    if(BoundaryEdges(j,4)==2)
        %plot([Nodes(BoundaryEdges(j,1),2),Nodes(BoundaryEdges(j,2),2),Nodes(BoundaryEdges(j,3),2)],[Nodes(BoundaryEdges(j,1),3),Nodes(BoundaryEdges(j,2),3),Nodes(BoundaryEdges(j,3),3)],"r*")
        numBound1 = numBound1+1;
    elseif(BoundaryEdges(j,4)==3)
        %plot([Nodes(BoundaryEdges(j,1),2),Nodes(BoundaryEdges(j,2),2),Nodes(BoundaryEdges(j,3),2)],[Nodes(BoundaryEdges(j,1),3),Nodes(BoundaryEdges(j,2),3),Nodes(BoundaryEdges(j,3),3)],"m*")
        numBound2 = numBound2+1;
    elseif(BoundaryEdges(j,4)==4)
        for i=1:3
            if((Nodes(BoundaryEdges(j,i),2)<3 && Nodes(BoundaryEdges(j,i),2)>2.75) && (Nodes(BoundaryEdges(j,i),3)<0.02 && Nodes(BoundaryEdges(j,i),3)>-0.02))
                plot(Nodes(BoundaryEdges(j,i),2),Nodes(BoundaryEdges(j,i),3),'m*')
                forceEdgeNodes(forceEdgeCouner)=BoundaryEdges(j,i)-1;
                forceEdgeCouner = forceEdgeCouner+1;
            end
        end
        numBound3 = numBound3+1;
    
    elseif(BoundaryEdges(j,4)==5)
         for i=1:3
            if((Nodes(BoundaryEdges(j,i),2)<0.3 && Nodes(BoundaryEdges(j,i),2)>0.1) && (Nodes(BoundaryEdges(j,i),3)<0.6 && Nodes(BoundaryEdges(j,i),3)>0.4))
                %plot(Nodes(BoundaryEdges(j,i),2),Nodes(BoundaryEdges(j,i),3),'g*')
                lockedNodesHole(lockedHoleCounter)=BoundaryEdges(j,i)-1;
                lockedHoleCounter = lockedHoleCounter+1;
            end
         end 
            numBound4 = numBound4+1;
    
    elseif(BoundaryEdges(j,4)==6)
         for i=1:3
        
        
            if((Nodes(BoundaryEdges(j,i),2)<4.5 && Nodes(BoundaryEdges(j,i),2)>4.3) && (Nodes(BoundaryEdges(j,i),3)<0.6 && Nodes(BoundaryEdges(j,i),3)>0.4))
                plot(Nodes(BoundaryEdges(j,i),2),Nodes(BoundaryEdges(j,i),3),'b*')
                forceNodeHoleRight(forceNodeHoleRightCounter)=BoundaryEdges(j,i)-1;
                forceNodeHoleRightCounter = forceNodeHoleRightCounter+1;
            end
         end
                    numBound5 = numBound5+1;     
    end
end
numBound2
numBound1
numBound3
numBound4
numBound5
for i=1:nNodes
    if((Nodes(i,2)<4.486 && Nodes(i,2)>4.484) && (Nodes(i,3)<0.485 && Nodes(i,3)>0.478))
        forceNodeHoleRight(end+1)= i-1;
    end

    if((Nodes(i,2)==0.231) && (Nodes(i,3)==0.4125))
        lockedNodesHole(end+1)= i-1;
    end

end

forceHoleRight = unique(forceNodeHoleRight);
lockedNodesHole = unique(lockedNodesHole);
forceNodeHoleRight = unique(forceNodeHoleRight);
forceEdgeNodes = unique(forceEdgeNodes);
lockedNodes=[
  16 0.000000
  17 0.000000
  18 0.000000
 364 0.000000
 365 0.000000
 368 0.000000
 371 0.000000
 380 0.000000
 381 0.000000
 384 0.000000
 387 0.000000
 426 0.000000
 427 0.000000
1692 0.000000
1693 0.000000
1694 0.000000
1695 0.000000];

lockedNodes = lockedNodes(:,1);
correctLockedNodes1=[];
c=1;
for i=1:numel(lockedNodes)
    if(Nodes(lockedNodes(i)+1,3)<0.05)
        plot(Nodes(lockedNodes(i)+1,2),Nodes(lockedNodes(i)+1,3),'r*')
        correctLockedNodes1(c)=lockedNodes(i);
        c=c+1;
    end
end
lockedNodes = sort([correctLockedNodes1';lockedNodesHole']);
lockedNodes = [lockedNodes,zeros(size(lockedNodes))];

forceHoleRight = unique(forceHoleRight);
forceHoleRight = [forceHoleRight',ones(size(forceHoleRight'))];
forceEdgeNodes = [forceEdgeNodes',ones(size(forceEdgeNodes'))*2];
forceNodes = ([forceHoleRight;forceEdgeNodes]);
forceNodes
for i=1:numel(lockedNodesHole)   

    plot(Nodes(lockedNodesHole(i)+1,2),Nodes(lockedNodesHole(i)+1,3),'g*')
     
end


plot(Nodes(493+1,2),Nodes(493+1,3),'*r')

plot(Nodes(495+1,2),Nodes(495+1,3),'*r')
findCrossNodes = [forceHoleRight(:,1),Nodes(forceHoleRight(:,1)+1,2:3)]

for i=1:nElements
    for j=1:nNodeElement-1

        node = Elements(i,j+1:j+2)+1;     
        xCoords=Nodes(node,2);
        yCoords=Nodes(node,3);
       
        plot(xCoords,yCoords,'-b')

    end
    node = Elements(i,[5,2])+1;
    xCoords=Nodes(node,2);
    yCoords=Nodes(node,3);

    plot(xCoords,yCoords,'-b')

end




theta = 15;
R = [cosd(theta),-sind(theta);
     sind(theta), cosd(theta)];


FL=1000;
mg=1500;


A=[9.7,6.3]; %sylinder punkt på bom, framme
B=[12.94,8.045]; %sylinder punkt outerBoom
C = [4,2.75];% sylinder punkt Tårn nedre
D = [6.1349,5.3158]; %Sylinder punkt bom bakre

vecbomToTow = C-D;
vecbomToTow = R'*vecbomToTow';
F2thetaTow = 360 - acosd((vecbomToTow(1))/norm(vecbomToTow));

vecBomToOut = B-A;
vecBomToOut = R'*vecBomToOut';
F2theta = acosd((vecBomToOut(1))/norm(vecBomToOut));


mgY = -mg*sind(theta);
mgX = -mg*cosd(theta);


F2 =  1.6012e+04;
F2V = [cosd(F2theta)*F2,sind(F2theta)*F2];

Fx = 1.6005e+04;

Fy = 1.2767e+03;

F2Tow = (mg*6 - F2V(1)*0.6575 - F2V(2)*8.2125 - Fx*3.2611 + Fy*12.1707)/(cosd(F2thetaTow)*0.284+sind(F2thetaTow)*4.7349)
%F2Tow =  2.5985e+04;
FxTow = Fx - F2V(1)+F2Tow*cosd(F2thetaTow)
%FxTow = 5.2394e+03;
FyTow = mg + F2V(2)*sind(F2thetaTow) + Fy - F2Tow*sind(F2thetaTow)
FyTow =  1.3543e+04;

GravityDir =[-0.2588 -0.9659];

FBearing = [Fx,Fy]

NodesForEndPos = [493,495];

posForOuter = [Nodes(NodesForEndPos(1)+1,2),Nodes(NodesForEndPos(2)+1,3)];

plot([4.4,0.5],'*g')


axis equal
hold off


fclose('all')