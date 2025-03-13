function  plotOuterBoom(xy,theta)

filename = "outerBoomMesh.dat";
fileID = fopen(filename,'r');
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
nElements = 1328;
nNodes = 1468;
AllElements=fscanf(fileID,"%d %d %d %d %d %d\n",[6,nElements])';

tline = fgetl(fileID);
Nodes=fscanf(fileID,"%d %f %f\n",[3,nNodes])';



Nodes = Nodes.*3;

holecoord = [6.21,3.3];
moveTo = -holecoord;
moveToSizeNodes = ones(size(Nodes()));
moveToSizeNodes(:,1) = moveToSizeNodes(:,1).*0;
moveToSizeNodes(:,2) = moveToSizeNodes(:,2).*moveTo(1);
moveToSizeNodes(:,3) = moveToSizeNodes(:,3).*moveTo(2);
Nodes = Nodes+moveToSizeNodes;


R = [cosd(theta),-sind(theta);
     sind(theta), cosd(theta)];

for i=1:size(Nodes,1)

    tempNode =  R*Nodes(i,2:3)';
    Nodes(i,2:3) = tempNode';

end

moveTo = xy;
moveToSizeNodes = ones(size(Nodes()));
moveToSizeNodes(:,1) = moveToSizeNodes(:,1).*0;
moveToSizeNodes(:,2) = moveToSizeNodes(:,2).*moveTo(1);
moveToSizeNodes(:,3) = moveToSizeNodes(:,3).*moveTo(2);
Nodes = Nodes+moveToSizeNodes;
nNodeElement=4;
tline = fgetl(fileID);


fclose(fileID);
Elements= [AllElements(:,1:5),zeros(nElements,1)];


grid on
hold on


forceEdgeNodes = getForceNodesOuter();


lockedNodes = getLockedNodesOuter();
for i=1:numel(forceEdgeNodes(:,1))   

    plot(Nodes(forceEdgeNodes(i,1)+1,2),Nodes(forceEdgeNodes(i,1)+1,3),'g*')

end

for i=1:numel(lockedNodes)   

    plot(Nodes(lockedNodes(i)+1,2),Nodes(lockedNodes(i)+1,3),'m*')

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

axis equal
hold off


fclose('all')