function [holeForOuter,forceNodeCoord] = plotMainBoom(xy,theta)
filename = "mainBoomMesh.dat";
fileID = fopen(filename,'r');
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
nElements = 2220;
nNodes = 2397;
AllElements=fscanf(fileID,"%d %d %d %d %d %d\n",[6,nElements])';

tline = fgetl(fileID);
Nodes=fscanf(fileID,"%d %f %f\n",[3,nNodes])';

Nodes = Nodes.*3;

holecoord = [0.6,1.5];
holeForOuter = [12.6;0];%After pull to zero
R = [cosd(theta),-sind(theta);
     sind(theta), cosd(theta)];
holeForOuter = R*holeForOuter;
holeForOuter(1) = holeForOuter(1) + xy(1);
holeForOuter(2) = holeForOuter(2) + xy(2);
%Move to 0,0
moveTo = -holecoord;
moveToSizeNodes = ones(size(Nodes()));
moveToSizeNodes(:,1) = moveToSizeNodes(:,1).*0;
moveToSizeNodes(:,2) = moveToSizeNodes(:,2).*moveTo(1);
moveToSizeNodes(:,3) = moveToSizeNodes(:,3).*moveTo(2);
Nodes = Nodes+moveToSizeNodes;

for i=1:size(Nodes,1)

    tempNode =  R*Nodes(i,2:3)';
    Nodes(i,2:3) = tempNode';

end

%Move to joint


moveToSizeNodes = ones(size(Nodes()));
moveToSizeNodes(:,1) = moveToSizeNodes(:,1).*0;
moveToSizeNodes(:,2) = moveToSizeNodes(:,2).*xy(1);
moveToSizeNodes(:,3) = moveToSizeNodes(:,3).*xy(2);
Nodes = Nodes+moveToSizeNodes;
nNodeElement=4;
tline = fgetl(fileID);


fclose(fileID);
Elements= [AllElements(:,1:5),zeros(nElements,1)];

grid on
hold on











forceNodes = getForceNodesMainBoom();
lockedNodes = getLockedNodesMainBoom();
for i=1:numel(lockedNodes)   

    plot(Nodes(lockedNodes(i)+1,2),Nodes(lockedNodes(i)+1,3),'g*')
     
end

for i=1:numel(forceNodes)   

    plot(Nodes(forceNodes(i)+1,2),Nodes(forceNodes(i)+1,3),'r*')
    forceNodeCoord(i,:) = [forceNodes(i),Nodes(forceNodes(i)+1,2),Nodes(forceNodes(i)+1,3)];
     
end


%% Plot Mesh
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
end