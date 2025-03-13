function  coordHoleForMainBoom = plotTower()

filename = "TowerMesh.dat";
fileID = fopen(filename,'r');
tline = fgetl(fileID)
tline = fgetl(fileID)
tline = fgetl(fileID)
tline = fgetl(fileID)
tline = fgetl(fileID)
nElements = 2460;
nNodes = 2566;
AllElements=fscanf(fileID,"%d %d %d %d %d %d\n",[6,nElements])';

tline = fgetl(fileID)
Nodes=fscanf(fileID,"%d %f %f\n",[3,nNodes])';

coordHoleForMainBoom = [1.4,5.6];

BoundaryEdges = getBoundaryEdgesTower();
%%

nNodeElement=4;
tline = fgetl(fileID)


fclose(fileID);
Elements= [AllElements(:,1:5),zeros(nElements,1)];



f1 = figure(1);
clf(f1,'reset');
grid on
hold on

forceNodeHoleRight=[];
forceNodeHoleRightCounter =1;

forceEdgeNodes = [];

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

forceEdgeNodes = unique(forceEdgeNodes);
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

axis equal
hold off
end