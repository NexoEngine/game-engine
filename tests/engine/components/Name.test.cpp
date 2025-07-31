///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date: Today lol
//  Description: Test file for the NameComponent
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "components/Name.hpp"
#include <string>
#include <limits>

namespace nexo::components {

class NameComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a default name component
        nameComp = NameComponent{};
    }
    
    NameComponent nameComp;
};

// Test default construction
TEST_F(NameComponentTest, DefaultConstruction) {
    NameComponent defaultName;
    EXPECT_TRUE(defaultName.name.empty());
    EXPECT_EQ(defaultName.name, "");
}

// Test construction with value
TEST_F(NameComponentTest, ValueConstruction) {
    NameComponent namedComp{"TestEntity"};
    EXPECT_EQ(namedComp.name, "TestEntity");
    EXPECT_FALSE(namedComp.name.empty());
}

// Test copy construction
TEST_F(NameComponentTest, CopyConstruction) {
    NameComponent original{"OriginalName"};
    NameComponent copy(original);
    
    EXPECT_EQ(copy.name, original.name);
    EXPECT_EQ(copy.name, "OriginalName");
    
    // Verify they are independent
    copy.name = "ModifiedName";
    EXPECT_EQ(original.name, "OriginalName");
    EXPECT_EQ(copy.name, "ModifiedName");
}

// Test move construction
TEST_F(NameComponentTest, MoveConstruction) {
    NameComponent original{"MovableName"};
    std::string originalValue = original.name;
    
    NameComponent moved(std::move(original));
    EXPECT_EQ(moved.name, originalValue);
    
    // Original should be in valid but unspecified state
    // For std::string, it's typically empty after move
    EXPECT_TRUE(original.name.empty() || original.name == originalValue);
}

// Test copy assignment
TEST_F(NameComponentTest, CopyAssignment) {
    NameComponent first{"FirstName"};
    NameComponent second{"SecondName"};
    
    second = first;
    EXPECT_EQ(second.name, "FirstName");
    EXPECT_EQ(first.name, "FirstName");
    
    // Verify independence
    second.name = "ModifiedSecond";
    EXPECT_EQ(first.name, "FirstName");
    EXPECT_EQ(second.name, "ModifiedSecond");
}

// Test move assignment
TEST_F(NameComponentTest, MoveAssignment) {
    NameComponent first{"FirstName"};
    NameComponent second{"SecondName"};
    
    second = std::move(first);
    EXPECT_EQ(second.name, "FirstName");
}

// Test empty string
TEST_F(NameComponentTest, EmptyString) {
    nameComp.name = "";
    EXPECT_TRUE(nameComp.name.empty());
    EXPECT_EQ(nameComp.name.length(), 0);
}

// Test single character
TEST_F(NameComponentTest, SingleCharacter) {
    nameComp.name = "A";
    EXPECT_EQ(nameComp.name, "A");
    EXPECT_EQ(nameComp.name.length(), 1);
}

// Test long string
TEST_F(NameComponentTest, LongString) {
    std::string longName(1000, 'x');
    nameComp.name = longName;
    
    EXPECT_EQ(nameComp.name, longName);
    EXPECT_EQ(nameComp.name.length(), 1000);
}

// Test very long string
TEST_F(NameComponentTest, VeryLongString) {
    std::string veryLongName(10000, 'a');
    nameComp.name = veryLongName;
    
    EXPECT_EQ(nameComp.name, veryLongName);
    EXPECT_EQ(nameComp.name.length(), 10000);
}

// Test special characters
TEST_F(NameComponentTest, SpecialCharacters) {
    std::vector<std::string> specialNames = {
        "Entity-01",
        "Entity_02",
        "Entity.03",
        "Entity@04",
        "Entity#05",
        "Entity$06",
        "Entity%07",
        "Entity&08",
        "Entity*09",
        "Entity(10)",
        "Entity[11]",
        "Entity{12}",
        "Entity<13>",
        "Entity\\14",
        "Entity/15",
        "Entity|16",
        "Entity'17",
        "Entity\"18\"",
        "Entity`19`",
        "Entity~20",
        "Entity!21",
        "Entity?22",
        "Entity:23",
        "Entity;24",
        "Entity,25",
        "Entity 26",  // Space
        "Entity\t27", // Tab
        "Entity\n28"  // Newline
    };
    
    for (const auto& specialName : specialNames) {
        nameComp.name = specialName;
        EXPECT_EQ(nameComp.name, specialName);
    }
}

// Test Unicode characters
TEST_F(NameComponentTest, UnicodeCharacters) {
    std::vector<std::string> unicodeNames = {
        "实体", // Chinese
        "エンティティ", // Japanese
        "개체", // Korean
        "Сущность", // Russian
        "كيان", // Arabic
        "οντότητα", // Greek
        "אֵנטִיטִי", // Hebrew
        "เอนทิตี", // Thai
        "👾🎮🎯", // Emojis
        "Entity™", // Trademark
        "Entity©", // Copyright
        "Entity®", // Registered
        "Entity°", // Degree
        "Entity±", // Plus-minus
        "Entity²", // Superscript
        "Entity½", // Fraction
        "Entity€", // Euro
        "Entity£", // Pound
        "Entity¥", // Yen
        "Entity§"  // Section
    };
    
    for (const auto& unicodeName : unicodeNames) {
        nameComp.name = unicodeName;
        EXPECT_EQ(nameComp.name, unicodeName);
    }
}

// Test string with null characters
TEST_F(NameComponentTest, StringWithNullCharacters) {
    std::string nameWithNull = "Entity";
    nameWithNull.push_back('\0');
    nameWithNull += "Hidden";
    
    nameComp.name = nameWithNull;
    EXPECT_EQ(nameComp.name.size(), 12); // "Entity" + null + "Hidden"
}

// Test string modification
TEST_F(NameComponentTest, StringModification) {
    nameComp.name = "Original";
    
    // Append
    nameComp.name += " Modified";
    EXPECT_EQ(nameComp.name, "Original Modified");
    
    // Insert
    nameComp.name.insert(8, " Very");
    EXPECT_EQ(nameComp.name, "Original Very Modified");
    
    // Replace
    size_t pos = nameComp.name.find("Very");
    if (pos != std::string::npos) {
        nameComp.name.replace(pos, 4, "Extremely");
    }
    EXPECT_EQ(nameComp.name, "Original Extremely Modified");
    
    // Erase
    nameComp.name.erase(8, 10); // Remove " Extremely"
    EXPECT_EQ(nameComp.name, "Original Modified");
    
    // Clear
    nameComp.name.clear();
    EXPECT_TRUE(nameComp.name.empty());
}

// Test comparison
TEST_F(NameComponentTest, Comparison) {
    NameComponent name1{"Alpha"};
    NameComponent name2{"Beta"};
    NameComponent name3{"Alpha"};
    
    EXPECT_TRUE(name1.name < name2.name);
    EXPECT_TRUE(name2.name > name1.name);
    EXPECT_TRUE(name1.name == name3.name);
    EXPECT_TRUE(name1.name != name2.name);
    EXPECT_TRUE(name1.name <= name3.name);
    EXPECT_TRUE(name2.name >= name1.name);
}

// Test case sensitivity
TEST_F(NameComponentTest, CaseSensitivity) {
    NameComponent lower{"entity"};
    NameComponent upper{"ENTITY"};
    NameComponent mixed{"Entity"};
    
    EXPECT_NE(lower.name, upper.name);
    EXPECT_NE(lower.name, mixed.name);
    EXPECT_NE(upper.name, mixed.name);
}

// Test whitespace handling
TEST_F(NameComponentTest, WhitespaceHandling) {
    // Leading whitespace
    nameComp.name = "   Entity";
    EXPECT_EQ(nameComp.name, "   Entity");
    
    // Trailing whitespace
    nameComp.name = "Entity   ";
    EXPECT_EQ(nameComp.name, "Entity   ");
    
    // Mixed whitespace
    nameComp.name = "  Entity  Name  ";
    EXPECT_EQ(nameComp.name, "  Entity  Name  ");
    
    // Only whitespace
    nameComp.name = "     ";
    EXPECT_EQ(nameComp.name, "     ");
    EXPECT_FALSE(nameComp.name.empty());
}

// Test numeric strings
TEST_F(NameComponentTest, NumericStrings) {
    std::vector<std::string> numericNames = {
        "0",
        "123",
        "-456",
        "3.14159",
        "1e10",
        "0x1A2B",
        "0777",
        "NaN",
        "Infinity",
        "-Infinity"
    };
    
    for (const auto& numericName : numericNames) {
        nameComp.name = numericName;
        EXPECT_EQ(nameComp.name, numericName);
    }
}

// Test aggregate initialization
TEST_F(NameComponentTest, AggregateInitialization) {
    NameComponent agg1{};
    EXPECT_TRUE(agg1.name.empty());
    
    NameComponent agg2{"Initialized"};
    EXPECT_EQ(agg2.name, "Initialized");
    
    NameComponent agg3{std::string("FromString")};
    EXPECT_EQ(agg3.name, "FromString");
}

// Test with different string types
TEST_F(NameComponentTest, DifferentStringTypes) {
    // From C-string
    nameComp.name = "C-String";
    EXPECT_EQ(nameComp.name, "C-String");
    
    // From string literal
    const char* literal = "String Literal";
    nameComp.name = literal;
    EXPECT_EQ(nameComp.name, "String Literal");
    
    // From char array
    char array[] = "Char Array";
    nameComp.name = array;
    EXPECT_EQ(nameComp.name, "Char Array");
    
    // From single char
    nameComp.name = std::string(1, 'X');
    EXPECT_EQ(nameComp.name, "X");
}

// Test memory efficiency with SSO (Small String Optimization)
TEST_F(NameComponentTest, SmallStringOptimization) {
    // Most implementations use SSO for strings up to 15-23 characters
    NameComponent small{"Short"}; // Should use SSO
    NameComponent large{std::string(100, 'L')}; // Should allocate
    
    // Both should work correctly regardless of SSO
    EXPECT_EQ(small.name, "Short");
    EXPECT_EQ(large.name.size(), 100);
    EXPECT_EQ(large.name[0], 'L');
    EXPECT_EQ(large.name[99], 'L');
}

// Test swap functionality
TEST_F(NameComponentTest, SwapFunctionality) {
    NameComponent first{"First"};
    NameComponent second{"Second"};
    
    std::swap(first.name, second.name);
    
    EXPECT_EQ(first.name, "Second");
    EXPECT_EQ(second.name, "First");
}

// Test multiple NameComponents
TEST_F(NameComponentTest, MultipleComponents) {
    std::vector<NameComponent> names;
    
    for (int i = 0; i < 100; ++i) {
        names.push_back(NameComponent{"Entity_" + std::to_string(i)});
    }
    
    // Verify all names are unique and correct
    for (size_t i = 0; i < names.size(); ++i) {
        EXPECT_EQ(names[i].name, "Entity_" + std::to_string(i));
    }
}

} // namespace nexo::components