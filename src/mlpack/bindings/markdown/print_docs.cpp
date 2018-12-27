/**
 * @file print_docs.cpp
 * @author Ryan Curtin
 *
 * Implementation of functions to print Markdown from documentation.
 */
#include "print_docs.hpp"

#include <mlpack/core/util/cli.hpp>
#include <mlpack/core/util/program_doc.hpp>
#include "binding_info.hpp"
#include "print_doc_functions.hpp"

// Make sure that this is defined.
#ifndef DOXYGEN_PREFIX
#define DOXYGEN_PREFIX "https://mlpack.org/docs/mlpack-git/doxygen/"
#endif

using namespace std;
using namespace mlpack;
using namespace mlpack::util;
using namespace mlpack::bindings;
using namespace mlpack::bindings::markdown;

void PrintDocs(const std::string& bindingName,
               const vector<string>& languages)
{
  ProgramDoc& programDoc = BindingInfo::GetProgramDoc(bindingName);

  CLI::RestoreSettings(bindingName);

  // First, for this section, print each of the names.
  for (size_t i = 0; i < languages.size(); ++i)
  {
    BindingInfo::Language() = languages[i];

    cout << "<!-- language: " << languages[i] << " -->" << endl;
    cout << "## " << GetBindingName(bindingName) << endl;
  }
  cout << endl;

  // Next we want to print the logical name of the binding (that's known by
  // ProgramInfo).
  cout << "```logicalName" << endl;
  cout << programDoc.programName << endl;
  cout << "```" << endl;
  cout << endl;

  // Next, print the PROGRAM_INFO() documentation for each language.
  for (size_t i = 0; i < languages.size(); ++i)
  {
    BindingInfo::Language() = languages[i];

    cout << "<!-- language: " << languages[i] << " -->" << endl;

    // We need to print the signature.

    // Now, iterate through each of the input options.
    cout << endl;
    cout << "### Input options" << endl;
    cout << endl;

    cout << "| ***name*** | ***type*** | ***description*** | ***default*** |"
        << endl;
    cout << "---------------------------------------------------------------"
        << endl;
    map<string, ParamData>& parameters = CLI::Parameters();
    for (map<string, ParamData>::const_iterator it = parameters.begin();
         it != parameters.end(); ++it)
    {
      if (!it->second.input)
        continue;

      // Print name, type, description, default.
      cout << "| ";
      cout << ParamString(it->second.name) << " | ";
      cout << ParamType(it->second) << " | ";
      cout << it->second.desc << " | "; // just a string
      cout << PrintDefault(it->second.name) << " |"; // needs implementation
      cout << endl;
    }
    cout << endl;

    // Next, iterate through the list of output options.
    cout << "### Output options" << endl;
    cout << endl;
    cout << programDoc.shortDocumentation << endl;
    cout << endl;
    cout << "| ***name*** | ***type*** | ***description*** |" << endl;
    cout << "-----------------------------------------------" << endl;
    for (map<string, ParamData>::const_iterator it = parameters.begin();
         it != parameters.end(); ++it)
    {
      if (it->second.input)
        continue;

      // Print name, type, description.
      cout << "| ";
      cout << ParamString(it->second.name) << " | ";
      cout << ParamType(it->second) << " | ";
      cout << it->second.desc << "|";
      cout << endl;
    }
    cout << endl;

    cout << "### Detailed documentation" << endl;
    cout << endl;
    cout << programDoc.documentation() << endl;
    cout << endl;

    cout << "### See also" << endl;
    cout << endl;
    for (size_t i = 0; i < programDoc.seeAlso.size(); ++i)
    {
      cout << " - " << "[" << programDoc.seeAlso[i].first << "](";

      // We need special handling of Doxygen information.
      if (programDoc.seeAlso[i].second.substr(0, 8) == "@doxygen")
        cout << DOXYGEN_PREFIX << programDoc.seeAlso[i].second.substr(9);
      else
        cout << programDoc.seeAlso[i].second;

      cout << ")" << endl;

    }
    cout << endl;
  }

  CLI::ClearSettings();
}
