#include "levenshtein.h"
#include <algorithm>
#include <vector>

int levenshteinDistance(const std::string &s1, const std::string &s2)
{
    const size_t len1 = s1.size();
    const size_t len2 = s2.size();
    std::vector<std::vector<int>> dp(len1 + 1, std::vector<int>(len2 + 1));

    for (size_t i = 0; i <= len1; ++i)
    {
        for (size_t j = 0; j <= len2; ++j)
        {
            if (i == 0)
            {
                dp[i][j] = j;
            }
            else if (j == 0)
            {
                dp[i][j] = i;
            }
            else
            {
                dp[i][j] =
                    std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1)});
            }
        }
    }

    return dp[len1][len2];
}

double calculateMatchPercentage(const std::string &str1, const std::string &str2)
{
    int maxLen = std::max(str1.length(), str2.length());
    if (maxLen == 0)
        return 100.0;
    int distance = levenshteinDistance(str1, str2);
    double similarity = (1.0 - static_cast<double>(distance) / maxLen) * 100;

    if (similarity >= 50.0)
    {
        return similarity;
    }
    return 0.0;
}
