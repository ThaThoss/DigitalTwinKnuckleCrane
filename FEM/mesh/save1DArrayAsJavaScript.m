function save1DArrayAsJavaScript(arr, functionName, filename)
    % Open file for writing
    fileID = fopen(filename, 'w');

 functionName = strcat('function ', functionName ,'() {\n');
    % Write JavaScript function header
    fprintf(fileID,functionName);
    fprintf(fileID, '  return [\n');

    % Write array elements with desired formatting
    for i = 1:length(arr)
        fprintf(fileID, '    %.6f', arr(i));
        if i < length(arr)
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