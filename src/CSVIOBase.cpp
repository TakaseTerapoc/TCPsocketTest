bool CSVIOBase::readCSVFile(const std::string& filename, std::vector<std::vector<std::string>>& csvdata) {
    std::vector<std::vector<std::string>> csvData;
    
    try
    {
        csv::CSVReader reader(filename, csv::CSVFormat().header_row(-1));
    }
    catch (const std::exception& e)
    {
        return false;
    }

    for (csv::CSVRow& row : reader) {
        std::vector<std::string> rowData;
        for (csv::CSVField& field : row) {
            rowData.push_back(field.get<>());
        }
        csvData.push_back(std::move(rowData));
    }

    return true;
}