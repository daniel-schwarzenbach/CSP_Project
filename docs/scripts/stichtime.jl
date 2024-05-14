using DelimitedFiles
using FilePathsBase

# Pfad zum Hauptordner
main_folder_path = p"../Euler/output"

# Funktion zum Anpassen der vierten Spalte
function adjust_time_column!(data)
    last_valid_time = 0.0
    for i in 1:size(data, 1)
        if data[i, 4] == 0
            last_valid_time = data[i-1, 4]
        else
            data[i, 4] += last_valid_time
        end
    end
end

# Funktion zum Bearbeiten aller Dateien in einem Ordner
function process_files_in_folder(folder_path)
    # Liste aller Dateien im Ordner
    files = readdir(folder_path, join=true)
    for file in files
        # Überprüfen, ob es sich um eine Datei handelt
        if isfile(file)
            # Daten aus der Datei lesen
            data = readdlm(file, ' ', Float64)
            # Anpassen der vierten Spalte
            adjust_time_column!(data)
            # Korrigierte Daten in die Datei zurückschreiben
            writedlm(file, data, ' ')
        elseif isdir(file)
            # Rekursiver Aufruf, wenn es sich um einen Unterordner handelt
            process_files_in_folder(file)
        end
    end
end

# Bearbeitung aller Dateien starten
process_files_in_folder(main_folder_path)