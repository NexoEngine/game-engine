\page accessibility_policy Accessibility Policy
# Accessibility Policy

| | |
|---|---|
| **Status** | Active |
| **Audience** | All contributors (core team and external) |
| **Last updated** | May 2026 |

This documentation is intended for all contributos, to ensure quality and accessibility throughout
the NEXO Engine project.

It is up to change, and we encourage all contributors to provide feedback and suggestions for improvement.

## Table of Contents

- [Accessibility Policy](#accessibility-policy)
  - [Reference Standards](#reference-standards)
  - [Accessibility Main Principles](#accessibility-main-principles)
    - [Equality](#equality)
    - [Inclusivity](#inclusivity)
    - [Usability](#usability)
    - [Flexibility](#flexibility)
    - [Awareness](#awareness)
  - [Accessibility Guidelines](#accessibility-guidelines)
    - [**A-101** - Take into account user preferences](#a-101---take-into-account-user-preferences)
    - [**A-102** - Respect contrast ratios](#a-102---respect-contrast-ratios)
    - [**A-103** - Provide text alternatives](#a-103---provide-text-alternatives)
    - [**A-104** - Ensure keyboard accessibility](#a-104---ensure-keyboard-accessibility)
    - [**A-105** - Provide sufficient time for interactions](#a-105---provide-sufficient-time-for-interactions)
    - [**A-106** - Avoid content that causes seizures](#a-106---avoid-content-that-causes-seizures)
    - [**A-107** - Provide clear and consistent navigation](#a-107---provide-clear-and-consistent-navigation)
    - [**A-108** - Use clear and simple language](#a-108---use-clear-and-simple-language)
    - [**A-109** - Provide multiple ways to access content](#a-109---provide-multiple-ways-to-access-content)
    - [**A-110** - Put yourself in the users' shoes](#a-110---put-yourself-in-the-users-shoes)



## Reference Standards

NEXO Engine's accessibility requirements are based on the following standards and guidelines:

- **Xbox Accessibility Guidelines (XAGs)** for gameplay, UI, and player experience in games built with the engine.  
  [Microsoft Xbox Accessibility Guidelines](https://learn.microsoft.com/en-us/gaming/accessibility/xbox-accessibility-guidelines)

- **WCAG 2.2 (Level AA)** for web-facing content, documentation, and UI elements delivered via web technologies.  
  [W3C WCAG 2.2](https://www.w3.org/TR/WCAG22/)

- **ATAG 2** for the NEXO Engine editor itself, as an authoring tool:
  - The editor must be accessible to disabled authors.
  - The editor must help authors produce accessible game content.  
  
  [W3C ATAG 2](https://www.w3.org/WAI/standards-guidelines/atag/)

- **RGAA‑APPS** as the French official reference for software and application accessibility (for compliance and evaluation in France). We are proudly french, after all! <br />
  [RGAA‑APPS](https://www.info.gouv.fr/accessibilite/le-cadre-legal/le-referentiel-pour-les-applications-et-logiciels)



## Accessibility Main Principles

1. <a id="equality"></a>**Equality**: Ensure all users, regardless of their abilities, disabilities, can use and enjoy using NEXO Engine as much as anyone else.

2. <a id="inclusivity"></a>**Inclusivity**: Design and develop features that corresponds to a wide range of users, including those with disabilities, to create an inclusive gaming experience.

3. <a id="usability"></a>**Usability**: Focus on creating an intuitive and user-friendly experience for all users, ensuring that accessibility features are easy to find and use. This concerns everyone. No one wants a bad user experience, right?

4. <a id="flexibility"></a>**Flexibility**: Provide options and settings that allow users to customize their experience according to their needs and preferences.

5. <a id="awareness"></a>**Awareness**: Promote awareness and understanding of accessibility issues among developers and the gaming community, fostering a culture of empathy and inclusivity.

## Accessibility Guidelines

### **A-101** - Take into account user preferences

Take into account user preferences defined at greater levels. Platform > OS > Game Engine.

For example, on Windows, user can set the scale factor of the UI, at the OS level. NEXO Engine takes into account this preference and scale the UI accordingly, without the user having to set it again in the engine.

Another example would be the color scheme. If the user has white mode enabled at the OS level, NEXO Engine should also use a light color scheme by default, unless the user explicitly set a different preference in the engine.

### **A-102** - Respect contrast ratios

Ensure that every visual component can be perceived equally for all users.

This means respecting the contrast ratios defined in the WCAG 2.2 guidelines for **Level AA**:

- **Normal text**: at least **4.5:1** contrast ratio between text and background.
- **Large text**: at least **3:1** contrast ratio.  
  - Large text is defined as 18 pt (24 px) or larger, or 14 pt (18.66 px) and bold.
- **UI components and graphical objects**: at least **3:1** contrast ratio against adjacent colors.  
  - This includes buttons, icons, borders, focus indicators, charts, and other essential graphics.

Logotypes and purely decorative elements are exempt from these requirements.

Use a color contrast checker (e.g. WebAIM Contrast Checker, Stark, or similar) to validate your design and implementation.

### **A-103** - Provide text alternatives

Provide text alternatives for non-text content, such as images, icons, and other visual elements. This allows users with visual impairments to understand the content through screen readers or other assistive technologies.

> [!WARNING]
> This is currently a work in progress on NEXO. <br />
> NEXO's UI libray is ImGui, which isn't screen reader compatible yet. <br />
> See https://github.com/ocornut/imgui/issues/8022 for more details.

### **A-104** - Ensure keyboard accessibility

Ensure that all interactive elements and functionalities can be accessed and operated using a keyboard alone. This includes navigation, form controls, and other interactive components.

> [!WARNING]
> This is currently a work in progress on NEXO. <br />
> Many shortcuts are available, but not all features are accessible via keyboard yet.

### **A-105** - Provide sufficient time for interactions

Ensure that users have enough time to read and interact with content, especially for time-sensitive tasks. Avoid time limits or provide options to adjust them.

Currently, NEXO Engine does not have any time-sensitive interactions, but this guideline should be kept in mind for future features.

### **A-106** - Avoid content that causes seizures

Avoid content that flashes at a high frequency. This includes flashing lights, rapidly changing images, or any content that could trigger seizures in users with photosensitive epilepsy.

### **A-107** - Provide clear and consistent navigation

Ensure that navigation is clear, consistent, and predictable throughout the engine and its documentation. This helps users understand where they are and how to find what they need.

### **A-108** - Use clear and simple language

Use clear and simple language in all documentation, UI text, and communication. Avoid jargon, technical terms, or complex sentences that may be difficult for some users to understand.

### **A-109** - Provide multiple ways to access content

Provide multiple ways to access content and features, such as through menus, shortcuts, or voice commands. This allows users to choose the method that works best for them.

NEXO currently implements keyboard shortcuts. We are open for any contribution to add voice commands or other input methods in the future (it would be especially cool when VR support will be implemented).

### **A-110** - Put yourself in the users' shoes

**Empathize** with users who have disabilities and consider their needs and challenges when designing and developing features.

**Understand** that accessibility is not just about compliance, but about creating a better experience for all users. Always ask yourself: "Would anyone be limited in their experience? To what extent? How can I make it better for them?"

**Listen** to feedback from users with disabilities and be open to making changes based on their input. They are the best source of information about their needs and preferences.

