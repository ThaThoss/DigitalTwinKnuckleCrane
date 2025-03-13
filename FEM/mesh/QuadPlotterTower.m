clc
clear all

filename = "TowerMesh.dat";
fileID = fopen(filename,'r');
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
nElements = 2460;
nNodes = 2566;
AllElements=fscanf(fileID,"%d %d %d %d %d %d\n",[6,nElements])';

tline = fgetl(fileID);
Nodes=fscanf(fileID,"%d %f %f\n",[3,nNodes])';
% Nodes(89,2:3) = [4.477,0.45];
% Nodes(1842,2:3) = [4.441,0.415];
% Nodes(501,2:3) = [4.455,0.425];
% Nodes(1841,2:3) = [4.468,0.435];


BoundaryEdges = getBoundaryEdgesTower();
%%

nNodeElement=4;
tline = fgetl(fileID);


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
    
    if(BoundaryEdges(j,4)==7)
        for i=1:3
           if((Nodes(BoundaryEdges(j,i),2)<1.5 && Nodes(BoundaryEdges(j,i),2)>1.3)  && (Nodes(BoundaryEdges(j,i),3)<5.7 && Nodes(BoundaryEdges(j,i),3)>5.5))
                plot(Nodes(BoundaryEdges(j,i),2),Nodes(BoundaryEdges(j,i),3),"m*")
                forceNodeHoleRight(forceNodeHoleRightCounter) = BoundaryEdges(j,i)-1;
                forceNodeHoleRightCounter = forceNodeHoleRightCounter+1;
           end
        
        end
                  
            
    end
end

c=1;
c2=1;
for i=1:nNodes
    if((Nodes(i,3)==0 ))
   
        lockedEdgeNodes(c)= i-1;
        c=c+1;
    end



    if((Nodes(i,2)==4) && ((Nodes(i,3)<3.1 && Nodes(i,3)>2.4)))
        forceEdgeNodes(c2)= i-1;
        c2=c2+1;
        
    end




end



forceNodeHoleRight = unique(forceNodeHoleRight);

forceEdgeNodes = unique(forceEdgeNodes)
SizeOfForceEdge = size(forceEdgeNodes)
forceEdgeNodes = [forceEdgeNodes,forceNodeHoleRight];

forceEdgeNodes = [forceEdgeNodes',zeros(size(forceEdgeNodes',1),2)];
forceEdgeNodes = sort(forceEdgeNodes);


lockedEdgeNodes = sort(lockedEdgeNodes)';
lockedEdgeNodes = [lockedEdgeNodes,zeros(size(lockedEdgeNodes))];

for i=1:numel(forceEdgeNodes(:,1))   

    plot(Nodes(forceEdgeNodes(i,1)+1,2),Nodes(forceEdgeNodes(i,1)+1,3),'g*')

end

for i=1:numel(lockedEdgeNodes(:,1))   

    plot(Nodes(lockedEdgeNodes(i,1)+1,2),Nodes(lockedEdgeNodes(i,1)+1,3),'r*')

end

%% Plot mesh
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



F2Tow =  2.5985e+04;
FxTow = 5.2394e+03;
FyTow =  1.3543e+04;
FTowTopVec = [FxTow,FyTow]/32

mg=1500;
C = [4,2.75];% sylinder punkt Tårn nedre
D = [6.1349,5.3158]; %Sylinder punkt bom bakre


vecbomToTow = D-C;
F2theta = acosd((vecbomToTow(1))/norm(vecbomToTow));
F2V = [cosd(F2theta)*F2Tow,sind(F2theta)*F2Tow]/5






%5239.4 1354.3


findCrossNodes = [forceNodeHoleRight',Nodes(forceNodeHoleRight+1,2:3)]

NodesForEndPos = [502,504];

posForBoom = [Nodes(NodesForEndPos(1)+1,2),Nodes(NodesForEndPos(2)+1,3)]

plot(Nodes(504+1,2),Nodes(504+1,3),'r*')
plot(Nodes(502+1,2),Nodes(502+1,3),'r*')
plot(posForBoom(1),posForBoom(2),'*r')
fclose('all');
axis equal
hold off