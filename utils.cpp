std::vector<std::string> splitString(std::string str, char delim)
{
    std::vector<std::string> result;
    std::string::iterator first = str.begin();
    for (auto itr = str.begin(); itr != str.end(); itr++)
    {
        if (*itr == delim)
        {
            result.push_back(std::string(first, itr));
            first = itr + 1;
            continue;
        }
    }
    result.push_back(std::string(first, str.end()));
    return result;
}