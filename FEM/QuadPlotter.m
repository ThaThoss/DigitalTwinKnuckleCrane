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
Nodes(89,2:3) = [4.477,0.45];
Nodes(1842,2:3) = [4.441,0.415];
Nodes(501,2:3) = [4.455,0.425];
Nodes(1841,2:3) = [4.468,0.435];



%%
elementProblemNr = [728]+1;%[568,588,589,591,604,607,620,636]+1;
nNodeElement=4
tline = fgetl(fileID)


%maxNode = max(max(AllElements(:,1:5)))
%minNode = min(min(AllElements(:,6:9)))
fclose(fileID);
Elements= [AllElements(:,1:5),zeros(nElements,1)];
%%
%hold on
edgeNodesLS = [ 13,  14,  63,  64,  65,  66]-1;
edgeNodesRS = [15,  16,  76,  77,  78,  79]-1;
setNodes = [edgeNodesLS',zeros(size(edgeNodesLS'))]  ;
forceNodes = [edgeNodesRS',100*ones(size(edgeNodesRS')),zeros(size(edgeNodesRS'))];
%%
%plot(Nodes(setNodes,2),Nodes(setNodes,3),'r*')
%hold off
f1 = figure(1);
clf(f1,'reset');
grid on
hold on
for i=1:nElements
    for j=1:nNodeElement-1
        
        node = Elements(i,j+1:j+2)+1;     
        xCoords=Nodes(node,2);
        yCoords=Nodes(node,3);
        %if((xCoords(1)>4.2))
        plot(xCoords,yCoords,'-b')
        %pause
        %plot(Nodes(node(1),2),Nodes(node(1),3),'*')
        %pause
        %end
        
        
    end
    node = Elements(i,[5,2])+1;
    xCoords=Nodes(node,2);
    yCoords=Nodes(node,3);
    %if((xCoords(1)>4.2))
    plot(xCoords,yCoords,'-b')
    %plot(Nodes(node(1),2),Nodes(node(1),3),'*')
    
    
    %end
   %
end
%elementPlotter(Elements,Nodes,729)


%node = Elements(1289,2:3)+1;
%plot(Nodes(node(1),2),Nodes(node(1),3),'m*')
%plot(Nodes(569,2),Nodes(569,3),'m*')
%node = Elements(1133,2:3)+1;
%plot(Nodes(node(2),2),Nodes(node(2),3),'b*')

% plot(Nodes(1:maxNode,2),Nodes(1:maxNode,3),'*')
% hold on
% 
% plot(Nodes(Elements(1,2)+1,2),Nodes(Elements(1,2)+1,3),'r*')
% plot(Nodes(Elements(1,3)+1,2),Nodes(Elements(1,3)+1,3),'m*')
% plot(Nodes(Elements(1,4)+1,2),Nodes(Elements(1,4)+1,3),'g*')
% plot(Nodes(Elements(1,5)+1,2),Nodes(Elements(1,5)+1,3),'y*')
% hold off
%%
listProbElem = [];
n=1;
% hold on
% for i=1:numel(elementProblemNr)
%     elNr = elementProblemNr(i)
%     for j=1:3
%     node = Elements(elNr,j+1:j+2)+1;     
%         xCoords=Nodes(node,2);
%         yCoords=Nodes(node,3);
%         if(xCoords(1)>4.2)
% 
%        plot(xCoords,yCoords,'-r')
%        plot(Nodes(node(1),2),Nodes(node(1),3),'*')
%        Nodes(node(1),:)
%        listProbElem(n) = elNr;
%        n=n+1;
% 
%         end
%        
%     end
%     node = Elements(elNr,[5,2])+1;
%     xCoords=Nodes(node,2);
%     yCoords=Nodes(node,3);
%     if(xCoords(1)>4.2)
%     plot(xCoords,yCoords,'-r')
%     nodeNr = node(1);
%     plot(Nodes(nodeNr,2),Nodes(nodeNr,3),'*')
%     end
%     
%   elNr  
%   
% end
% unique(listProbElem)
%plot(Nodes(node(1),2),Nodes(node(1),3),'r*')
%plot(Nodes(1088,2),Nodes(1088,3),'y*')


%plot(Nodes(node(1),2),Nodes(node(1),3),'y*')
axis equal
hold off


fclose('all')
