clc
clear all

filename = "outerBoomMesh.dat";
fileID = fopen(filename,'r');
tline = fgetl(fileID)
tline = fgetl(fileID)
tline = fgetl(fileID)
tline = fgetl(fileID)
tline = fgetl(fileID)
nElements = 1328;
nNodes = 1468;
AllElements=fscanf(fileID,"%d %d %d %d %d %d\n",[6,nElements])';

tline = fgetl(fileID)
Nodes=fscanf(fileID,"%d %f %f\n",[3,nNodes])';



BoundaryEdges = getBoundaryEdgesouter();
%%
elementProblemNr = [728]+1; %[568,588,589,591,604,607,620,636]+1;
nNodeElement=4
tline = fgetl(fileID)


%maxNode = max(max(AllElements(:,1:5)))
%minNode = min(min(AllElements(:,6:9)))
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
        for i=1:3
            if((Nodes(BoundaryEdges(j,i),2)<0.34 && Nodes(BoundaryEdges(j,i),2)>0.16) && (Nodes(BoundaryEdges(j,i),3)<0.11 && Nodes(BoundaryEdges(j,i),3)>0.01))
            
                forceEdgeNodes(forceEdgeCouner)=BoundaryEdges(j,i)-1;
                forceEdgeCouner = forceEdgeCouner+1;
               % plot([Nodes(BoundaryEdges(j,i),2)],[Nodes(BoundaryEdges(j,i),3)],"g*")
            end
        end
        numBound1 = numBound1+1;
    elseif(BoundaryEdges(j,4)==3)
        %plot([Nodes(BoundaryEdges(j,1),2),Nodes(BoundaryEdges(j,2),2),Nodes(BoundaryEdges(j,3),2)],[Nodes(BoundaryEdges(j,1),3),Nodes(BoundaryEdges(j,2),3),Nodes(BoundaryEdges(j,3),3)],"m*")
        numBound2 = numBound2+1;
    elseif(BoundaryEdges(j,4)==4)
        for i=1:3
            if((Nodes(BoundaryEdges(j,i),2)<2.15 && Nodes(BoundaryEdges(j,i),2)>2) && (Nodes(BoundaryEdges(j,i),3)<1.17 && Nodes(BoundaryEdges(j,i),3)>1.04))
                %plot(Nodes(BoundaryEdges(j,i),2),Nodes(BoundaryEdges(j,i),3),'m*')
                lockedNodesHole(lockedHoleCounter)=BoundaryEdges(j,i)-1;
                lockedHoleCounter = lockedHoleCounter+1;
            end
        end
        numBound3 = numBound3+1;
        
    end
end
numBound2
numBound1
numBound3
lockedEdgeNodes = [];
deleteNode=[0,0];
a=(1.035-0.99)/(1.9-1.79827);
y0=0.94-a*1.7;
c=1;
for i=1:nNodes
    if((Nodes(i,2)<4.486 && Nodes(i,2)>4.484) && (Nodes(i,3)<0.485 && Nodes(i,3)>0.478))
        forceNodeHoleRight(end+1)= i-1;
    end

    if((Nodes(i,2)==2.07) && (Nodes(i,3)==1.138))
        lockedNodesHole(end+1)= i-1;
    end
    if((Nodes(i,2)==2.0925) && (Nodes(i,3)==1.13))
        lockedNodesHole(end+1)= i-1;
    end

    if((Nodes(i,2)==0.21) && (Nodes(i,3)==0.02))
        forceEdgeNodes(end+1)= i-1;
        
    end
    if((Nodes(i,2)<0.217 && Nodes(i,2)<0.217>0.2165) && (Nodes(i,3)<0.0464 && Nodes(i,3)>0.0462 ))
        deleteNode(1) = i-1;
        
    end
    if((Nodes(i,2)==0.2908) && (Nodes(i,3)==0.06313))
        deleteNode(2) = i-1;
        
    end
    if(Nodes(i,2)<1.8 && Nodes(i,2)>1.7)
        
        y=a*Nodes(i,2)+y0;
        if(Nodes(i,3)>y)
            lockedEdgeNodes(c) = i-1;
            c=c+1;
        end
        

    end


end

forceHoleRight = unique(forceNodeHoleRight);
lockedNodesHole = unique(lockedNodesHole);
forceNodeHoleRight = unique(forceNodeHoleRight);
forceEdgeNodes = unique(forceEdgeNodes);


forceEdgeNodes = [forceEdgeNodes',zeros(size(forceEdgeNodes',1),2)];
forceEdgeNodes(forceEdgeNodes==119,:)=[0,0,0];
forceEdgeNodes(forceEdgeNodes==116,:)=[0,0,0];
forceEdgeNodes = sort(forceEdgeNodes);
forceEdgeNodes = forceEdgeNodes(3:end,:);

lockedNodes = sort([lockedNodesHole,lockedEdgeNodes]');
lockedNodes = [lockedNodes,zeros(size(lockedNodes))]
for i=1:numel(forceEdgeNodes(:,1))   

    plot(Nodes(forceEdgeNodes(i,1)+1,2),Nodes(forceEdgeNodes(i,1)+1,3),'g*')

end

for i=1:numel(lockedNodesHole)   

    plot(Nodes(lockedNodesHole(i)+1,2),Nodes(lockedNodesHole(i)+1,3),'m*')

end



for i=1:numel(lockedEdgeNodes)   

    plot(Nodes(lockedEdgeNodes(i)+1,2),Nodes(lockedEdgeNodes(i)+1,3),'r*')

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
theta = 30;
R = [cosd(theta),-sind(theta);
     sind(theta), cosd(theta)];

A=[9.7,6.3];%sylinder punkt på bom, framme
B=[12.94,8.045];%sylinder punkt outerBoom
vec = A-B;
vec = R'*vec';
theta = acosd((vec(1))/norm(vec));

%F Load:
FL=1000;
mg=1500/2;


F2 = (mg*1.5+5.55*FL)/(sind(180-theta)*0.949+cosd(180-theta)*0.389)
Fx =  cosd(180-theta)*F2
Fy = -sind(180-theta)*F2 +mg +FL


GravityDir =[ -0.5000 -0.8660];
FLVec = [  500.0000 -866.0254];

% F2 =
% 
%    1.6012e+04
% 
% 
% Fx =
% 
%    1.6005e+04
% 
% 
% Fy =
% 
%    1.2767e+03