#include "aiws/text_processor.hpp"

namespace aiws
{

    std::vector<TokenInfo> TextProcessor::tokenize(const std::string &text)
    {
        // TODO: produce normalized tokens with source and paragraph information.
        std::vector<TokenInfo> tokens;

        // Tracks the paragraph number
        std::size_t paragraph = 0;

        // Tracks the token start/end
        std::size_t tokenStart = 0;
        std::size_t tokenEnd = 0;

        // Used to track the condition for a new paragraph
        bool lastCharWasNewLine = false;

        // Used to indicate a wait for a new paragraph after conditions are met
        bool waitingForNewParagraph = true;

        std::string currentWord = "";

        for (std::size_t i = 0; i < text.length(); i++)
        {
            // Get char and convert to lowercase
            char curChar = std::tolower(text.at(i));

            // Check to see if you're on a new paragraph
            // Must be new line after previous char was new line and can't already be the start of a new paragraph
            if (curChar == '\n' && lastCharWasNewLine && !waitingForNewParagraph)
            {
                paragraph++;                   // iterate paragraph count
                lastCharWasNewLine = false;    // We need new conditions for a paragraph
                waitingForNewParagraph = true; // We now just wait for the paragraph to start
            }
            // Check to see if we could have a new paragraph
            else if (curChar == '\n' && !lastCharWasNewLine)
            {
                lastCharWasNewLine = true;

                if (currentWord != "")
                {
                    // Separator handling
                    tokenEnd = i;
                    TokenInfo currentToken = {currentWord, tokenStart, tokenEnd, paragraph};

                    tokens.push_back(currentToken);

                    currentWord = "";
                }
            }
            // Pieces out our characters/numbers
            else if ((curChar >= 48 && curChar <= 57) || (curChar >= 97 && curChar <= 122))
            {
                if (waitingForNewParagraph)
                {
                    waitingForNewParagraph = false;
                }

                if (currentWord == "")
                {
                    tokenStart = i;
                }

                lastCharWasNewLine = false;
                currentWord = currentWord + curChar;

                // Handling of last token
                if (i == text.length() - 1)
                {
                    tokenEnd = i + 1;
                    TokenInfo currentToken = {currentWord, tokenStart, tokenEnd, paragraph};

                    tokens.push_back(currentToken);
                }
            }
            // The next case is that we have a separator, this ensures we don't create a new token for consecutive separators
            else if (!currentWord.empty())
            {
                tokenEnd = i;
                TokenInfo currentToken = {currentWord, tokenStart, tokenEnd, paragraph};

                tokens.push_back(currentToken);

                currentWord = "";
            }
        }
        return tokens;
    }

    std::vector<std::string> TextProcessor::terms(const std::string &text)
    {
        // TODO: return the normalized terms represented by the input text.

        std::vector<std::string> normalizationResults;
        std::string normalizedText = "";
        bool lastCharWasSeparator = true;

        for (std::size_t i = 0; i < text.length(); i++)
        {

            // Get char and convert to lowercase
            char curChar = std::tolower(text.at(i));

            if ((curChar >= 48 && curChar <= 57) || (curChar >= 97 && curChar <= 122))
            {
                if (lastCharWasSeparator && !normalizedText.empty())
                {
                    normalizationResults.push_back(normalizedText);
                    normalizedText = "";
                }

                normalizedText = normalizedText + curChar;
                lastCharWasSeparator = false;
            }
            else
            {
                lastCharWasSeparator = true;
            }
        }

        if (!normalizedText.empty())
        {
            normalizationResults.push_back(normalizedText);
        }

        return normalizationResults;
    }

    std::string TextProcessor::normalize(const std::string &text)
    {
        // TODO: return the normalized form of the input text.

        std::string normalizedText = "";
        bool lastCharWasSeparator = true;

        for (std::size_t i = 0; i < text.length(); i++)
        {

            // Get char and convert to lowercase
            char curChar = std::tolower(text.at(i));

            if ((curChar >= 48 && curChar <= 57) || (curChar >= 97 && curChar <= 122))
            {
                if (lastCharWasSeparator && !normalizedText.empty())
                {
                    normalizedText = normalizedText + " ";
                }

                normalizedText = normalizedText + curChar;
                lastCharWasSeparator = false;
            }
            else
            {
                lastCharWasSeparator = true;
            }
        }
        return normalizedText;
    }

    static std::string join(const std::vector<TokenInfo> &tokens,
                            std::size_t begin,
                            std::size_t end)
    {
        // TODO: join the requested token range into normalized text.

        std::string result = "";

        if (end < begin)
        {
            return {};
        }
        else
        {
            for (std::size_t i = begin; i < end; i++)
            {
                result += tokens.at(i).token;

                if (i != end - 1)
                {
                    result += " ";
                }
            }

            return result;
        }
    }

    static std::string join(const std::vector<std::string> &tokens,
                            std::size_t begin,
                            std::size_t end)
    {
        // TODO: join the requested term range into normalized text.

        std::string result = "";

        if (end < begin)
        {
            return {};
        }
        else
        {
            for (std::size_t i = begin; i < end; i++)
            {
                result += tokens.at(i);

                if (i != end - 1)
                {
                    result += " ";
                }
            }

            return result;
        }
    }

} // namespace aiws
