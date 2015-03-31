/**
 * Reactive
 *
 * (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <string>
#include <vector>
#include <set>
#include <sstream>

namespace reactive {

    /** Class with specialized methods for string management
     *
     * splitting, substitution, slicing...
     */
    class string
    {
    private:
        /** A private constructor prevents from instantiation */
        string() {}

    public:

        /** Split a string a let the slice in order as they were found
         *
         * @param delimiter_  Delimiting string between which the input string will be split
         * @param content_    Input string to split
         * @param parts_      Output set of strings that will contain the results of the split search
         */
        static void split(const std::string& delimiter_, const std::string& content_, std::vector<std::string>& parts_)
        {
            // These have to be in size_t type to avoid bad assignements
            std::size_t match_index = 0, pos = 0;

            //--- Scanning the rest of the string
            while (match_index != std::string::npos)
            {
                // If content_ is empty or delimiter is not present in the content string
                // Then match_index will be at npos
                match_index = content_.find(delimiter_, pos);

                // Extracting the string from the last position 'pos' to the found
                // length (match_index-pos)
                if (match_index != std::string::npos)
                {
                    parts_.push_back(content_.substr(pos, match_index-pos));
                    pos = match_index + delimiter_.size();
                }
                else
                {
                    parts_.push_back(content_.substr(pos));
                }
            }
        }

        /** Split a string in a set ordered by string order (alphanumeric)
         *
         * @param delimiter_  Delimiting string between which the input string will be split
         * @param content_    Input string to split
         * @param parts_      Output set of strings that will contain the results of the split search
         */
        static void split(const std::string& delimiter_, const std::string& content_, std::set<std::string>& parts_)
        {
            // These have to be in size_t type to avoid bad assignements
            std::size_t match_index = 0, pos = 0;

            //--- Scanning the rest of the string
            while (match_index != std::string::npos)
            {
                // If content_ is empty or delimiter is not present in the content string
                // Then match_index will be at npos
                match_index = content_.find(delimiter_, pos);

                // Extracting the string from the last position 'pos' to the found
                // length (match_index-pos)
                if (match_index != std::string::npos)
                {
                    parts_.insert(content_.substr(pos, match_index-pos));
                    pos = match_index + delimiter_.size();
                }
                else
                {
                    parts_.insert(content_.substr(pos));
                }
            }
        }

        /**  Chops off the delimiter and everything that follows (destructively)
         * returns everything after the delimiter
         *
         * @param delimiter_        Separator string between head an tail.
         * @param keep_delimiter_   Boolean if true we keep the delimiter inside the tail string else we do not keep it anywhere.
         * @param subject_to_tail_  Input to string to work on. It will contain the tail slice.
         * @param tail_output_      Output string containing the tail slice.
         *
         */
        static void tailSlice(const std::string& delimiter_, std::string& subject_to_head_, std::string& tail_output_, bool keep_delimiter_)
        {
            std::size_t delimiter_location = subject_to_head_.find(delimiter_);

            if (delimiter_location != std::string::npos)
            {
                int start_index;
                if (keep_delimiter_)
                {
                    start_index = delimiter_location;
                }
                else
                {
                    start_index = delimiter_location + delimiter_.length();
                }

                // getting all characters until the end from the strat_index
                tail_output_     = subject_to_head_.substr(start_index);
                subject_to_head_ = subject_to_head_.substr(0, delimiter_location);
            }
            else
            {
                // subject_to_head_ is unchanged
                tail_output_ = "";
            }
        }

        /**  Chops off the delimiter and everything that precedes (destructively)
         * returns everything before the delimiter
         *
         * @param delimiter_        Separator string between head an tail
         * @param subject_to_tail_  Input to string to work on. It will contain the tail slice.
         * @param head_output_      Output string containing the head slice
         *
         */
        static void headSlice(const std::string& delimiter_, std::string& subject_to_tail_, std::string& head_output_)
        {
            std::size_t delimiter_location = subject_to_tail_.find(delimiter_);

            if (delimiter_location != std::string::npos)
            {
                head_output_ = subject_to_tail_.substr(0, delimiter_location);
                subject_to_tail_ = subject_to_tail_.substr( delimiter_location + delimiter_.length() );
            }
            else
            {
                // subject_to_head_ does not change
                head_output_ = "";
            }
        }

        /** Substitue all match_ with a replacement_ string inside an input string replace_str_
         *
         * @param match_  String to replace
         * @param replacement_  String to use to overwrite all the matches
         * @param replace_str_  Input string to work on. It will remain untouched
         * @return a new string with the occurence replaced
         */
        static std::string substitute(const std::string& match_, const std::string& replacement_, const std::string& replace_str_)
        {
            std::string output = "";

            // Split the input in a vector
            std::vector<std::string> vstr;
            split(match_, replace_str_, vstr);

            // And fill with the replacement string
            for(std::size_t k = 0; k < vstr.size(); k++)
            {
                if(k != 0)
                {
                    output.append(replacement_);
                }
                output.append(vstr[k]);
            }

            return output;
        }

        /**
         * Convert string to NumberType
         *
         * @param value_    The string number
         * @return          The number
         */
        template<typename NumberType, typename SourceType>
        static NumberType to_number(const SourceType& value_)
        {
            std::stringstream iss(value_);
            NumberType x;

            if (!(iss >> x))
            {
                return (NumberType)0;
            }

            return x;
        }
    }; // end of string class
} // end of reactive namespace
