function saveArrayAsJavaScript(arr,functionName, filename)
    % Open file for writing
    fileID = fopen(filename, 'w');
    functionName = strcat('function ', functionName ,'() {\n');
    % Write JavaScript function header
    fprintf(fileID,functionName);
    fprintf(fileID, '  return [\n');

    % Write array elements
    for i = 1:size(arr, 1)
        fprintf(fileID, '    [');
        for j = 1:size(arr, 2)
            fprintf(fileID, ' %.6f', arr(i, j));
            if j < size(arr, 2)
                fprintf(fileID, ',');
            end
        end
        fprintf(fileID, ' ]');
        if i < size(arr, 1)
            fprintf(fileID, ',');
        end
        fprintf(fileID, '\n');
    end

    % Write JavaScript function footer
    fprintf(fileID, '  ];\n');
    fprintf(fileID, '}\n');

    % Close the file
    fclose(fileID);
end