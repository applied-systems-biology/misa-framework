/**
 * Copyright by Ruman Gerst
 * Research Group Applied Systems Biology - Head: Prof. Dr. Marc Thilo Figge
 * https://www.leibniz-hki.de/en/applied-systems-biology.html
 * HKI-Center for Systems Biology of Infection
 * Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Insitute (HKI)
 * Adolf-Reichwein-Straße 23, 07745 Jena, Germany
 *
 * This code is licensed under BSD 2-Clause
 * See the LICENSE file provided with this code for the full license.
 */

#pragma once


#include <string>
#include <memory>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>

namespace markdown::impl {

    /**
     * A markdown element
     */
    struct element {

        /**
         * Renders the markdown element as string
         * @return
         */
        virtual std::string to_string() const = 0;

    };

    /**
     * An element that wraps around another element
     * @tparam Element
     */
    template<typename Element, typename Base>
    struct wrapper : public Base {
        std::unique_ptr<Element> content;

        wrapper() = default;

        explicit wrapper(std::unique_ptr<Element> t_content) : content(std::move(t_content)) {

        }
    };

    /**
     * An inline-element
     */
    struct span : public element {
    };

    /**
     * An element that forms its own paragraph
     */
    struct paragraph : public element {
    };

    /**
     * An inline text
     */
    struct text : public span {
        std::string content;

        text() = default;

        explicit text(std::string t_content) : content(std::move(t_content)) {
        }

        std::string to_string() const override {
            std::string result = content;
            for(const std::string &escape : { "\\", "`", "*", "_", "#" }) {
                boost::replace_all(result, escape, "\\" + escape);
            }
            return result;
        }
    };

    /**
     * An italic-formatted text
     */
    struct italic : public wrapper<text, span> {

        using wrapper<text, span>::wrapper;

        std::string to_string() const override {
            if(content->to_string().empty())
                return "";
            return "*" + content->to_string() + "*";
        }
    };

    /**
     * A bold-formatted text
     */
    struct bold : public wrapper<text, span> {

        using wrapper<text, span>::wrapper;

        std::string to_string() const override {
            if(content->to_string().empty())
                return "";
            return "**" + content->to_string() + "**";
        }
    };

    /**
     * Inline code
     */
    struct inline_code : public wrapper<text, span> {

        using wrapper<text, span>::wrapper;

        std::string to_string() const override {
            if(content->content.empty())
                return "";
            return "`" + content->content + "`";
        }
    };

    /**
     * A link
     */
    struct link : public span {
        std::string content;
        std::string target;

        link() = default;

        explicit link(std::string t_content, std::string t_target) :
                content(std::move(t_content)), target(std::move(t_target)) {

        }

        std::string to_string() const override {
            if(content != target)
                return "[" + content + "](" + target + ")";
            else
                return target;
        }
    };

    /**
     * An image
     */
    struct image : public span {
        std::string alt;
        std::string target;

        image() = default;

        explicit image(std::string t_target, std::string t_alt = "") :
                alt(std::move(t_alt)), target(std::move(t_target)) {

        }

        std::string to_string() const override {
            return "![" + alt + "](" + target + ")";
        }
    };

    /**
     * An item list
     */
    struct itemize : public paragraph {
        std::vector<std::unique_ptr<span>> content;

        itemize() = default;

        explicit itemize(std::vector<std::unique_ptr<span>> t_content) : content(std::move(t_content)) {
        }

        template<typename Span>
        void operator+=(Span entry) {
            content.emplace_back(std::move(entry));
        }

        template<typename Span>
        itemize &operator+(Span entry) {
            *this += std::move(entry);
            return *this;
        }

        template<typename Span, typename ...Args>
        void addAll(Span &&entry, Args &&... args) {
            *this += std::forward<Span>(entry);
            if constexpr (sizeof...(Args) > 0) {
                addAll(std::forward<Args>(args)...);
            }
        }

        std::string to_string() const override {
            std::string output = "";
            for (const auto &item : content) {
                output += "* " + item->to_string() + "\n";
            }
            return output;
        }
    };

    /**
     * An enumerated list
     */
    struct enumerate : public paragraph {
        std::vector<std::unique_ptr<span>> content;

        enumerate() = default;

        explicit enumerate(std::vector<std::unique_ptr<span>> t_content) : content(std::move(t_content)) {

        }
        template<typename Span>
        void operator+=(Span entry) {
            content.emplace_back(std::move(entry));
        }

        template<typename Span>
        enumerate &operator+(Span entry) {
            *this += std::move(entry);
            return *this;
        }

        template<typename Span, typename ...Args>
        void addAll(Span &&entry, Args &&... args) {
            *this += std::forward<Span>(entry);
            if constexpr (sizeof...(Args) > 0) {
                addAll(std::forward<Args>(args)...);
            }
        }

        std::string to_string() const override {
            std::string output = "";
            for (size_t i = 0; i < content.size(); ++i) {
                output += std::to_string(i + 1) + ". " + content.at(i)->to_string() + "\n";
            }
            return output;
        }
    };

    /**
     * A horizontal ruler
     */
    struct hruler : public paragraph {
        std::string to_string() const override {
            return "---";
        }
    };

    /**
     * A block of code
     */
    struct code : public wrapper<text, paragraph> {
        std::string language;

        code() = default;

        code(std::unique_ptr<text> t_content, std::string t_language) :
                wrapper<text, paragraph>(std::move(t_content)), language(std::move(t_language)) {

        }

        std::string to_string() const override {
            return "```" + language + "\n" + content->content + "\n```";
        }
    };

    /**
     * A heading
     */
    struct heading : public wrapper<text, paragraph> {

        int depth = 0;

        heading() = default;

        explicit heading(std::unique_ptr<text> t_content, int t_depth = 0) :
                wrapper<text, paragraph>::wrapper(std::move(t_content)), depth(t_depth) {

        }

        std::string to_string() const override {
            if (depth == 0) {
                return content->to_string() + "\n" + std::string(content->to_string().length(), '=');
            } else if (depth == 1) {
                return content->to_string() + "\n" + std::string(content->to_string().length(), '-');
            } else {
                return std::string(depth + 1, '#') + " " + content->to_string();
            }
        }
    };

    /**
     * Paragraph that wraps around a span
     */
    struct span_paragraph : public paragraph {

        std::vector<std::unique_ptr<span>> content;

        template<typename Span>
        void operator+=(Span entry) {
            content.emplace_back(std::move(entry));
        }

        template<typename Span>
        span_paragraph &operator+(Span entry) {
            *this += std::move(entry);
            return *this;
        }

        template<typename Span, typename ...Args>
        void addAll(Span &&entry, Args &&... args) {
            *this += std::forward<Span>(entry);
            if constexpr (sizeof...(Args) > 0) {
                addAll(std::forward<Args>(args)...);
            }
        }

        std::string to_string() const override {
            std::string output;
            for (const auto &s : content) {
                output += s->to_string();
            }

            constexpr int line_width = 80;

            if (output.length() <= line_width)
                return output;

            // Apply word wrapping
            std::vector<std::string> words;
            boost::split(words, output, boost::is_any_of(" "));
            output = "";
            std::string current_line;

            for (const auto &word : words) {
                if (word.length() > line_width) {
                    output += current_line + "\n";
                    output += word;
                    current_line = "";
                } else if (current_line.length() + word.length() > line_width) {
                    output += current_line + "\n";
                    current_line = word + " ";
                } else {
                    current_line += word + " ";
                }
            }

            output += current_line;
            boost::trim_if(output, boost::is_any_of(" \n"));

            return output;
        }
    };

    /**
     * A markdown table
     */
    template<size_t Columns>
    struct table : public paragraph {
        using row = std::array<std::unique_ptr<span>, Columns>;
        std::vector<row> content;

        void operator+=(row entry) {
            content.emplace_back(std::move(entry));
        }

        table<Columns> &operator+(row entry) {
            *this += std::move(entry);
            return *this;
        }

        std::string to_string() const override {
            if(content.empty())
                return "";

            // First detect the row and column lengths
            std::array<size_t, Columns> column_lengths;
            for(size_t i = 0; i < column_lengths.size(); ++i) {
                column_lengths.at(i) = 0;
            }
            for(const row &r : content) {
                for(size_t i = 0; i < r.size(); ++i) {
                    std::string as_string =  r.at(i)->to_string();
                    column_lengths.at(i) = std::max(column_lengths.at(i), as_string.length() + 2);
                }
            }

            // At which position that pipes must be set
            std::array<size_t, Columns> column_guards;
            for(size_t i = 0; i < column_lengths.size(); ++i) {
                if(i == 0) {
                    column_guards.at(i) = 1 + column_lengths.at(i);
                }
                else {
                    column_guards.at(i) = 1 + column_lengths.at(i) + column_guards.at(i - 1);
                }
            }

            std::stringstream result;

            bool first_row = true;
            for(const auto &r : content) {

                result << "|";
                size_t line_counter = 1;

                for(size_t col = 0; col < r.size(); ++col) {
                    std::string written = r.at(col)->to_string();
                    result << " " << written << " ";
                    line_counter += 2 + written.length();
                    while(line_counter < column_guards.at(col)) {
                        result << " ";
                        ++line_counter;
                    }
                    result << "|";
                    ++line_counter;
                }

                result << "\n";

                if(first_row) {
                    // Write the separator
                    result << "|";
                    for(size_t i = 1; i < column_guards.at(column_guards.size() - 1) + 1; ++i) {
                        bool is_column = std::find(column_guards.begin(), column_guards.end(), i) != column_guards.end();
                        if(is_column)
                            result << "|";
                        else
                            result << "-";
                    }
                    result << "\n";
                    first_row = false;
                }
            }



            return result.str();
        }
    };

    /**
     *  A markdown document builder
     */
    struct document : public element {

        std::vector<std::unique_ptr<paragraph>> content;

        template<typename Paragraph>
        void operator+=(std::unique_ptr<Paragraph> entry) {
            static_assert(std::is_base_of<paragraph, Paragraph>::value, "You can only add paragraphs");
            content.emplace_back(std::move(entry));
        }

        template<typename Paragraph>
        document &operator+(std::unique_ptr<Paragraph> entry) {
            *this += std::move(entry);
            return *this;
        }

        std::string to_string() const override {
            std::string output;
            for (const auto &paragraph : content) {
                output += paragraph->to_string() + "\n\n";
            }
            return output;
        }

    };
}

namespace markdown {
    using document = markdown::impl::document;

    template<size_t Columns>
    using table = markdown::impl::table<Columns>;

    inline std::unique_ptr<markdown::impl::span_paragraph> paragraph() {
        return std::make_unique<markdown::impl::span_paragraph>();
    }
    
    template<typename Span, typename ...Args>
    inline std::unique_ptr<markdown::impl::span_paragraph> paragraph(Span &&span, Args &&... args) {
        auto result = std::make_unique<markdown::impl::span_paragraph>();
        result->addAll(std::forward<Span>(span), std::forward<Args>(args)...);
        return result;
    }

    inline std::unique_ptr<markdown::impl::span_paragraph> paragraph(std::unique_ptr<markdown::impl::span> span) {
        auto result = std::make_unique<markdown::impl::span_paragraph>();
        *result += std::move(span);
        return result;
    }

    inline std::unique_ptr<markdown::impl::itemize> itemize() {
        return std::make_unique<markdown::impl::itemize>();
    }

    template<typename Span, typename ...Args>
    inline std::unique_ptr<markdown::impl::itemize> itemize(Span &&span, Args &&... args) {
        auto result = std::make_unique<markdown::impl::itemize>();
        result->addAll(std::forward<Span>(span), std::forward<Args>(args)...);
        return result;
    }

    inline std::unique_ptr<markdown::impl::itemize> itemize(std::unique_ptr<markdown::impl::span> span) {
        auto result = std::make_unique<markdown::impl::itemize>();
        *result += std::move(span);
        return result;
    }

    inline std::unique_ptr<markdown::impl::enumerate> enumerate() {
        return std::make_unique<markdown::impl::enumerate>();
    }

    template<typename Span, typename ...Args>
    inline std::unique_ptr<markdown::impl::enumerate> enumerate(Span &&span, Args &&... args) {
        auto result = std::make_unique<markdown::impl::enumerate>();
        result->addAll(std::forward<Span>(span), std::forward<Args>(args)...);
        return result;
    }

    inline std::unique_ptr<markdown::impl::enumerate> enumerate(std::unique_ptr<markdown::impl::span> span) {
        auto result = std::make_unique<markdown::impl::enumerate>();
        *result += std::move(span);
        return result;
    }

    inline std::unique_ptr<markdown::impl::text> text(std::string content) {
        return std::make_unique<markdown::impl::text>(std::move(content));
    }

    inline std::unique_ptr<markdown::impl::heading> heading(std::string title, int level = 0) {
        return std::make_unique<markdown::impl::heading>(text(std::move(title)), level);
    }

    inline std::unique_ptr<markdown::impl::heading> heading1(std::string title) {
        return heading(std::move(title), 0);
    }

    inline std::unique_ptr<markdown::impl::heading> heading2(std::string title) {
        return heading(std::move(title), 1);
    }

    inline std::unique_ptr<markdown::impl::italic> italic(std::string content) {
        return std::make_unique<markdown::impl::italic>(text(std::move(content)));
    }

    inline std::unique_ptr<markdown::impl::bold> bold(std::string content) {
        return std::make_unique<markdown::impl::bold>(text(std::move(content)));
    }

    inline std::unique_ptr<markdown::impl::code> code(std::string content, std::string language = "") {
        return std::make_unique<markdown::impl::code>(text(std::move(content)), std::move(language));
    }

    inline std::unique_ptr<markdown::impl::inline_code> inline_code(std::string content) {
        return std::make_unique<markdown::impl::inline_code>(text(std::move(content)));
    }

    inline std::unique_ptr<markdown::impl::link> link(std::string content, std::string url) {
        return std::make_unique<markdown::impl::link>(std::move(content), std::move(url));
    }

    inline std::unique_ptr<markdown::impl::link> link(const std::string &url) {
        return link(url, url);
    }

    inline std::unique_ptr<markdown::impl::hruler> hruler() {
        return std::make_unique<markdown::impl::hruler>();
    }

    template<typename ...Args>
    inline typename markdown::impl::table<sizeof...(Args)>::row row(Args... args) {
        typename markdown::impl::table<sizeof...(Args)>::row result = { std::move(args)... };
        return result;
    }

    template<typename ValueType>
    inline std::unique_ptr<ValueType> cut(ValueType &target) {
        return std::make_unique<ValueType>(std::move(target));
    }
}


