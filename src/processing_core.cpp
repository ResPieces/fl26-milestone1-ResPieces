#include "aiws/processing_core.hpp"
#include "aiws/text_processor.hpp"
#include <stdexcept>

namespace aiws
{

    struct ProcessingCore::Impl
    {
        // TODO: define the internal state used by the processing core.
        std::vector<Chunk> chunks;
        CorpusIndex index;
    };

    ProcessingCore::ProcessingCore() : impl_(std::make_unique<Impl>()) {}

    ProcessingCore::~ProcessingCore() = default;

    ProcessingCore::ProcessingCore(ProcessingCore &&) noexcept = default;

    ProcessingCore &ProcessingCore::operator=(ProcessingCore &&) noexcept = default;

    std::string ProcessingCore::normalize(const std::string &text)
    {
        // TODO: return the normalized form of the input text.
        return TextProcessor::normalize(text);
    }

    void ProcessingCore::rebuild(const Workspace &workspace)
    {
        // TODO: rebuild the processing state from the workspace.

        // Stores the ID's we have already seen
        std::vector<std::string> documentIds;

        // First loop validates document ID's
        for (const Document &doc : workspace.documents())
        {
            // Checks new ID against existing list
            for (std::string id : documentIds)
            {
                if (doc.id() == id)
                {
                    throw std::invalid_argument("Repeated Document ID's - Corpus remains unchanged");
                }
            }

            documentIds.push_back(doc.id());
        }

        Chunker chunker;
        std::vector<Chunk> newChunks;
        std::size_t docOrder = 0;

        for (const Document &doc : workspace.documents())
        {
            std::vector<Chunk> tempChunks = chunker.chunk(doc, docOrder);
            newChunks.insert(newChunks.end(), tempChunks.begin(), tempChunks.end());
            docOrder++;
        }

        CorpusIndex newIndex;
        newIndex.build(newChunks);

        impl_->chunks = std::move(newChunks);
        impl_->index = std::move(newIndex);
    }

    const std::vector<Chunk> &ProcessingCore::chunks() const noexcept
    {
        // TODO: return the chunks currently stored by the processing core.
        return impl_->chunks;
    }

    std::size_t ProcessingCore::chunk_count() const noexcept
    {
        // TODO: return the number of stored chunks.
        return impl_->chunks.size();
    }

    std::size_t ProcessingCore::document_frequency(const std::string &term) const
    {
        // TODO: return the document frequency for the requested term.
        return impl_->index.document_frequency(term);
    }

    std::size_t ProcessingCore::term_frequency(const std::string &term,
                                               const std::string &chunk_id) const
    {
        // TODO: return the term frequency for the requested chunk.
        return impl_->index.term_frequency(term, chunk_id);
    }

    /*
    SearchResult elements:

    std::string chunk_id;
    std::string document_id;
    std::size_t chunk_sequence{};
    std::string text;
    double score{};
    std::size_t matched_terms{};
    */

    std::vector<SearchResult> ProcessingCore::search(const std::string &query, int k) const
    {
        // TODO: return the ranked results for the requested query.

        if (k < 0)
        {
            throw std::invalid_argument("Invalid Input: k is negative");
        }

        std::vector<std::string> queryTerms = normalizeQuery(query);

        return {};
    }

    // Helper Function designed to remove repeated query words
    std::vector<std::string> ProcessingCore::normalizeQuery(const std::string &query) const
    {
        std::vector<std::string> rawTerms = TextProcessor::terms(query);

        std::vector<std::string> cleanTerms;

        for (int i = 0; i < rawTerms.size(); i++)
        {
            bool termAlreadyAdded = false;
            for (int j = 0; j < cleanTerms.size(); j++)
            {
                if (cleanTerms.at(j) == rawTerms.at(i))
                {
                    termAlreadyAdded = true;
                }
            }
            if (!termAlreadyAdded)
            {
                cleanTerms.push_back(rawTerms.at(i));
            }
        }

        return cleanTerms;
    }

    /*
    ContextItem elements:

    std::string chunk_id;
    std::string document_id;
    std::size_t chunk_sequence{};
    std::string text;
    std::size_t token_count{};
    double score{};
    bool truncated{};
    */

    std::vector<ContextItem> ProcessingCore::build_context(const std::string &,
                                                           int,
                                                           std::size_t) const
    {
        // TODO: build bounded context for the requested query.
        return {};
    }

} // namespace aiws
