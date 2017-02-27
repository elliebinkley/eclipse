/*
 * ValidateBinaryTree.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: USER
 *
 *  This exercise is taken from  "Cracking the Coding Interview". I solved it on the web, then took the solutions and put it in this
 * suite of solved puzzles. See https://www.hackerrank.com/domains/tutorials/cracking-the-coding-interview under login:lburley ;
 * Trees: Is This a Binary Search Tree?
 * Here I had to add infrastructure that was provided on the web, e.g. a binary search tree with data.
 *
 * From Hackerrank:
 *  Validate a binary tree.
 *  For the purposes of this challenge, we define a binary search tree to be a binary tree with the following ordering properties:
 *      The  value of every node in a node's left subtree is less than the data value of that node.
 *      The  value of every node in a node's right subtree is greater than the data value of that node.
 *      Given the root node of a binary tree, can you determine if it's also a binary search tree?
 *  Complete the function in your editor below, which has  parameter: a pointer to the root of a binary tree.
 *  It must return a boolean denoting whether or not the binary tree is a binary search tree. You may have to write one or more helper functions to complete this challenge.
 *
 *  Note: We do not consider a binary tree to be a binary search tree if it contains duplicate values.
 *  Input Format
 *        You are not responsible for reading any input from stdin.
 *  Hidden code stubs will assemble a binary tree and pass its root node to your function as an argument.
 *  Constraints
 *  Output Format
 *  You are not responsible for printing any output to stdout. Your function must return true if the tree is a binary search tree; otherwise, it must return false. Hidden code stubs will print this result as a Yes or No answer on a new line.
 *  Sample Input
 *  tree
 *  Sample Output
 *  Yes
 *  Explanation
 *  The tree in the diagram satisfies the ordering property for a Binary Search Tree, so we print Yes.
 *  9
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256 257 258 259 260 261 262 263 264 265 266 267 268 269 270 271 272 273 274 275 276 277 278 279 280 281 282 283 284 285 286 287 288 289 290 291 292 293 294 295 296 297 298 299 300 301 302 303 304 305 306 307 308 309 310 311 312 313 314 315 316 317 318 319 320 321 322 323 324 325 326 327 328 329 330 331 332 333 334 335 336 337 338 339 340 341 342 343 344 345 346 347 348 349 350 351 352 353 354 355 356 357 358 359 360 361 362 363 364 365 366 367 368 369 370 371 372 373 374 375 376 377 378 379 380 381 382 383 384 385 386 387 388 389 390 391 392 393 394 395 396 397 398 399 400 401 402 403 404 405 406 407 408 409 410 411 412 413 414 415 416 417 418 419 420 421 422 423 424 425 426 427 428 429 430 431 432 433 434 435 436 437 438 439 440 441 442 443 444 445 446 447 448 449 450 451 452 453 454 455 456 457 458 459 460 461 462 463 464 465 466 467 468 469 470 471 472 473 474 475 476 477 478 479 480 481 482 483 484 485 486 487 488 489 490 491 492 493 494 495 496 497 498 499 500 501 504 503 504 505 506 507 508 509 510 511 512 513 514 515 516 517 518 519 520 521 522 523 524 525 526 527 528 529 530 531 532 533 534 535 536 537 538 539 540 541 542 543 544 545 546 547 548 549 550 551 552 553 554 555 556 557 558 559 560 561 562 563 564 565 566 567 568 569 570 571 572 573 574 575 576 577 578 579 580 581 582 583 584 585 586 587 588 589 590 591 592 593 594 595 596 597 598 599 600 601 602 603 604 605 606 607 608 609 610 611 612 613 614 615 616 617 618 619 620 621 622 623 624 625 626 627 628 629 630 631 632 633 634 635 636 637 638 639 640 641 642 643 644 645 646 647 648 649 650 651 652 653 654 655 656 657 658 659 660 661 662 663 664 665 666 667 668 669 670 671 672 673 674 675 676 677 678 679 680 681 682 683 684 685 686 687 688 689 690 691 692 693 694 695 696 697 698 699 700 701 702 703 704 705 706 707 708 709 710 711 712 713 714 715 716 717 718 719 720 721 722 723 724 725 726 727 728 729 730 731 732 733 734 735 736 737 738 739 740 741 742 743 744 745 746 747 748 749 750 751 752 753 754 755 756 757 758 759 760 761 762 763 764 765 766 767 768 769 770 771 772 773 774 775 776 777 778 779 780 781 782 783 784 785 786 787 788 789 790 791 792 793 794 795 796 797 798 799 800 801 802 803 804 805 806 807 808 809 810 811 812 813 814 815 816 817 818 819 820 821 822 823 824 825 826 827 828 829 830 831 832 833 834 835 836 837 838 839 840 841 842 843 844 845 846 847 848 849 850 851 852 853 854 855 856 857 858 859 860 861 862 863 864 865 866 867 868 869 870 871 872 873 874 875 876 877 878 879 880 881 882 883 884 885 886 887 888 889 890 891 892 893 894 895 896 897 898 899 900 901 902 903 904 905 906 907 908 909 910 911 912 913 914 915 916 917 918 919 920 921 922 923 924 925 926 927 928 929 930 931 932 933 934 935 936 937 938 939 940 941 942 943 944 945 946 947 948 949 950 951 952 953 954 955 956 957 958 959 960 961 962 963 964 965 966 967 968 969 970 971 972 973 974 975 976 977 978 979 980 981 982 983 984 985 986 987 988 989 990 991 992 993 994 995 996 997 998 999 1000 1001 1002 1003 1004 1005 1006 1007 1008 1009 1010 1011 1012 1013 1014 1015 1016 1017 1018 1019 1020 1021 1022 1023";
 *  yes
 *  9
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256 257 258 259 260 261 262 263 264 265 266 267 268 269 270 271 272 273 274 275 276 277 278 279 280 281 282 283 284 285 286 287 288 289 290 291 292 293 294 295 296 297 298 299 300 301 302 303 304 305 306 307 308 309 310 311 312 313 314 315 316 317 318 319 320 321 322 323 324 325 326 327 328 329 330 331 332 333 334 335 336 337 338 339 340 341 342 343 344 345 346 347 348 349 350 351 352 353 354 355 356 357 358 359 360 361 362 363 364 365 366 367 368 369 370 371 372 373 374 375 376 377 378 379 380 381 382 383 384 385 386 387 388 389 390 391 392 393 394 395 396 397 398 399 400 401 402 403 404 405 406 407 408 409 410 411 412 413 414 415 416 417 418 419 420 421 422 423 424 425 426 427 428 429 430 431 432 433 434 435 436 437 438 439 440 441 442 443 444 445 446 447 448 449 450 451 452 453 454 455 456 457 458 459 460 461 462 463 464 465 466 467 468 469 470 471 472 473 474 475 476 477 478 479 480 481 482 483 484 485 486 487 488 489 490 491 492 493 494 495 496 497 498 499 500 501 502 503 504 505 506 507 508 509 510 511 512 513 514 515 516 517 518 519 520 521 522 523 524 525 526 527 528 529 530 531 532 533 534 535 536 537 538 539 540 541 542 543 544 545 546 547 548 549 550 551 552 553 554 555 556 557 558 559 560 561 562 563 564 565 566 567 568 569 570 571 572 573 574 575 576 577 578 579 580 581 582 583 584 585 586 587 588 589 590 591 592 593 594 595 596 597 598 599 600 601 602 603 604 605 606 607 608 609 610 611 612 613 614 615 616 617 618 619 620 621 622 623 624 625 626 627 628 629 630 631 632 633 634 635 636 637 638 639 640 641 642 643 644 645 646 647 648 649 650 651 652 653 654 655 656 657 658 659 660 661 662 663 664 665 666 667 668 669 670 671 672 673 674 675 676 677 678 679 680 681 682 683 684 685 686 687 688 689 690 691 692 693 694 695 696 697 698 699 700 701 702 703 704 705 706 707 708 709 710 711 712 713 714 715 716 717 718 719 720 721 722 723 724 725 726 727 728 729 730 731 732 733 734 735 736 737 738 739 740 741 742 743 744 745 746 747 748 749 750 751 752 753 754 755 756 757 758 759 760 761 762 763 764 765 766 767 768 769 770 771 772 773 774 775 776 777 778 779 780 781 782 783 784 785 786 787 788 789 790 791 792 793 794 795 796 797 798 799 800 801 802 803 804 805 806 807 808 809 810 811 812 813 814 815 816 817 818 819 820 821 822 823 824 825 826 827 828 829 830 831 832 833 834 835 836 837 838 839 840 841 842 843 844 845 846 847 848 849 850 851 852 853 854 855 856 857 858 859 860 861 862 863 864 865 866 867 868 869 870 871 872 873 874 875 876 875 878 879 880 881 882 883 884 885 886 887 888 889 890 891 892 893 894 895 896 897 898 899 900 901 902 903 904 905 906 907 908 909 910 911 912 913 914 915 916 917 918 919 920 921 922 923 924 925 926 927 928 929 930 931 932 933 934 935 936 937 938 939 940 941 942 943 944 945 946 947 948 949 950 951 952 953 954 955 956 957 958 959 960 961 962 963 964 965 966 967 968 969 970 971 972 973 974 975 976 977 978 979 980 981 982 983 984 985 986 987 988 989 990 991 992 993 994 995 996 997 998 999 1000 1001 1002 1003 1004 1005 1006 1007 1008 1009 1010 1011 1012 1013 1014 1015 1016 1017 1018 1019 1020 1021 1022 1023
 *  no
 *  4
 *  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 17 16 18 19 20 21 22 23 24 25 26 27 28 29 30 31
 *  no
 *  Hidden stub code will pass a root argument to the function below. Complete the function to solve the challenge. Hint: you may want to write one or more helper functions.
 *  The Node struct is defined as follows:
 *      struct Node
 *      {
 *           int data;
 *           Node* left;
 *           Node* right;
 *      }
 */
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include "Utilities.hpp"

class BST;
// forward declaration

// input data taken from hackerrank...
static const bool isDataSetValid0 = true;
static const std::string dataSet0 =
        "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256 257 258 259 260 261 262 263 264 265 266 267 268 269 270 271 272 273 274 275 276 277 278 279 280 281 282 283 284 285 286 287 288 289 290 291 292 293 294 295 296 297 298 299 300 301 302 303 304 305 306 307 308 309 310 311 312 313 314 315 316 317 318 319 320 321 322 323 324 325 326 327 328 329 330 331 332 333 334 335 336 337 338 339 340 341 342 343 344 345 346 347 348 349 350 351 352 353 354 355 356 357 358 359 360 361 362 363 364 365 366 367 368 369 370 371 372 373 374 375 376 377 378 379 380 381 382 383 384 385 386 387 388 389 390 391 392 393 394 395 396 397 398 399 400 401 402 403 404 405 406 407 408 409 410 411 412 413 414 415 416 417 418 419 420 421 422 423 424 425 426 427 428 429 430 431 432 433 434 435 436 437 438 439 440 441 442 443 444 445 446 447 448 449 450 451 452 453 454 455 456 457 458 459 460 461 462 463 464 465 466 467 468 469 470 471 472 473 474 475 476 477 478 479 480 481 482 483 484 485 486 487 488 489 490 491 492 493 494 495 496 497 498 499 500 501 502 503 504 505 506 507 508 509 510 511 512 513 514 515 516 517 518 519 520 521 522 523 524 525 526 527 528 529 530 531 532 533 534 535 536 537 538 539 540 541 542 543 544 545 546 547 548 549 550 551 552 553 554 555 556 557 558 559 560 561 562 563 564 565 566 567 568 569 570 571 572 573 574 575 576 577 578 579 580 581 582 583 584 585 586 587 588 589 590 591 592 593 594 595 596 597 598 599 600 601 602 603 604 605 606 607 608 609 610 611 612 613 614 615 616 617 618 619 620 621 622 623 624 625 626 627 628 629 630 631 632 633 634 635 636 637 638 639 640 641 642 643 644 645 646 647 648 649 650 651 652 653 654 655 656 657 658 659 660 661 662 663 664 665 666 667 668 669 670 671 672 673 674 675 676 677 678 679 680 681 682 683 684 685 686 687 688 689 690 691 692 693 694 695 696 697 698 699 700 701 702 703 704 705 706 707 708 709 710 711 712 713 714 715 716 717 718 719 720 721 722 723 724 725 726 727 728 729 730 731 732 733 734 735 736 737 738 739 740 741 742 743 744 745 746 747 748 749 750 751 752 753 754 755 756 757 758 759 760 761 762 763 764 765 766 767 768 769 770 771 772 773 774 775 776 777 778 779 780 781 782 783 784 785 786 787 788 789 790 791 792 793 794 795 796 797 798 799 800 801 802 803 804 805 806 807 808 809 810 811 812 813 814 815 816 817 818 819 820 821 822 823 824 825 826 827 828 829 830 831 832 833 834 835 836 837 838 839 840 841 842 843 844 845 846 847 848 849 850 851 852 853 854 855 856 857 858 859 860 861 862 863 864 865 866 867 868 869 870 871 872 873 874 875 876 877 878 879 880 881 882 883 884 885 886 887 888 889 890 891 892 893 894 895 896 897 898 899 900 901 902 903 904 905 906 907 908 909 910 911 912 913 914 915 916 917 918 919 920 921 922 923 924 925 926 927 928 929 930 931 932 933 934 935 936 937 938 939 940 941 942 943 944 945 946 947 948 949 950 951 952 953 954 955 956 957 958 959 960 961 962 963 964 965 966 967 968 969 970 971 972 973 974 975 976 977 978 979 980 981 982 983 984 985 986 987 988 989 990 991 992 993 994 995 996 997 998 999 1000 1001 1002 1003 1004 1005 1006 1007 1008 1009 1010 1011 1012 1013 1014 1015 1016 1017 1018 1019 1020 1021 1022 1023";

static const bool isDataSetValid1 = false;
static const std::string dataSet1 =
        "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256 257 258 259 260 261 262 263 264 265 266 267 268 269 270 271 272 273 274 275 276 277 278 279 280 281 282 283 284 285 286 287 288 289 290 291 292 293 294 295 296 297 298 299 300 301 302 303 304 305 306 307 308 309 310 311 312 313 314 315 316 317 318 319 320 321 322 323 324 325 326 327 328 329 330 331 332 333 334 335 336 337 338 339 340 341 342 343 344 345 346 347 348 349 350 351 352 353 354 355 356 357 358 359 360 361 362 363 364 365 366 367 368 369 370 371 372 373 374 375 376 377 378 379 380 381 382 383 384 385 386 387 388 389 390 391 392 393 394 395 396 397 398 399 400 401 402 403 404 405 406 407 408 409 410 411 412 413 414 415 416 417 418 419 420 421 422 423 424 425 426 427 428 429 430 431 432 433 434 435 436 437 438 439 440 441 442 443 444 445 446 447 448 449 450 451 452 453 454 455 456 457 458 459 460 461 462 463 464 465 466 467 468 469 470 471 472 473 474 475 476 477 478 479 480 481 482 483 484 485 486 487 488 489 490 491 492 493 494 495 496 497 498 499 500 501 504 503 504 505 506 507 508 509 510 511 512 513 514 515 516 517 518 519 520 521 522 523 524 525 526 527 528 529 530 531 532 533 534 535 536 537 538 539 540 541 542 543 544 545 546 547 548 549 550 551 552 553 554 555 556 557 558 559 560 561 562 563 564 565 566 567 568 569 570 571 572 573 574 575 576 577 578 579 580 581 582 583 584 585 586 587 588 589 590 591 592 593 594 595 596 597 598 599 600 601 602 603 604 605 606 607 608 609 610 611 612 613 614 615 616 617 618 619 620 621 622 623 624 625 626 627 628 629 630 631 632 633 634 635 636 637 638 639 640 641 642 643 644 645 646 647 648 649 650 651 652 653 654 655 656 657 658 659 660 661 662 663 664 665 666 667 668 669 670 671 672 673 674 675 676 677 678 679 680 681 682 683 684 685 686 687 688 689 690 691 692 693 694 695 696 697 698 699 700 701 702 703 704 705 706 707 708 709 710 711 712 713 714 715 716 717 718 719 720 721 722 723 724 725 726 727 728 729 730 731 732 733 734 735 736 737 738 739 740 741 742 743 744 745 746 747 748 749 750 751 752 753 754 755 756 757 758 759 760 761 762 763 764 765 766 767 768 769 770 771 772 773 774 775 776 777 778 779 780 781 782 783 784 785 786 787 788 789 790 791 792 793 794 795 796 797 798 799 800 801 802 803 804 805 806 807 808 809 810 811 812 813 814 815 816 817 818 819 820 821 822 823 824 825 826 827 828 829 830 831 832 833 834 835 836 837 838 839 840 841 842 843 844 845 846 847 848 849 850 851 852 853 854 855 856 857 858 859 860 861 862 863 864 865 866 867 868 869 870 871 872 873 874 875 876 877 878 879 880 881 882 883 884 885 886 887 888 889 890 891 892 893 894 895 896 897 898 899 900 901 902 903 904 905 906 907 908 909 910 911 912 913 914 915 916 917 918 919 920 921 922 923 924 925 926 927 928 929 930 931 932 933 934 935 936 937 938 939 940 941 942 943 944 945 946 947 948 949 950 951 952 953 954 955 956 957 958 959 960 961 962 963 964 965 966 967 968 969 970 971 972 973 974 975 976 977 978 979 980 981 982 983 984 985 986 987 988 989 990 991 992 993 994 995 996 997 998 999 1000 1001 1002 1003 1004 1005 1006 1007 1008 1009 1010 1011 1012 1013 1014 1015 1016 1017 1018 1019 1020 1021 1022 1023";

static const bool isDataSetValid2 = false;
static const std::string dataSet2 =
        "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256 257 258 259 260 261 262 263 264 265 266 267 268 269 270 271 272 273 274 275 276 277 278 279 280 281 282 283 284 285 286 287 288 289 290 291 292 293 294 295 296 297 298 299 300 301 302 303 304 305 306 307 308 309 310 311 312 313 314 315 316 317 318 319 320 321 322 323 324 325 326 327 328 329 330 331 332 333 334 335 336 337 338 339 340 341 342 343 344 345 346 347 348 349 350 351 352 353 354 355 356 357 358 359 360 361 362 363 364 365 366 367 368 369 370 371 372 373 374 375 376 377 378 379 380 381 382 383 384 385 386 387 388 389 390 391 392 393 394 395 396 397 398 399 400 401 402 403 404 405 406 407 408 409 410 411 412 413 414 415 416 417 418 419 420 421 422 423 424 425 426 427 428 429 430 431 432 433 434 435 436 437 438 439 440 441 442 443 444 445 446 447 448 449 450 451 452 453 454 455 456 457 458 459 460 461 462 463 464 465 466 467 468 469 470 471 472 473 474 475 476 477 478 479 480 481 482 483 484 485 486 487 488 489 490 491 492 493 494 495 496 497 498 499 500 501 502 503 504 505 506 507 508 509 510 511 512 513 514 515 516 517 518 519 520 521 522 523 524 525 526 527 528 529 530 531 532 533 534 535 536 537 538 539 540 541 542 543 544 545 546 547 548 549 550 551 552 553 554 555 556 557 558 559 560 561 562 563 564 565 566 567 568 569 570 571 572 573 574 575 576 577 578 579 580 581 582 583 584 585 586 587 588 589 590 591 592 593 594 595 596 597 598 599 600 601 602 603 604 605 606 607 608 609 610 611 612 613 614 615 616 617 618 619 620 621 622 623 624 625 626 627 628 629 630 631 632 633 634 635 636 637 638 639 640 641 642 643 644 645 646 647 648 649 650 651 652 653 654 655 656 657 658 659 660 661 662 663 664 665 666 667 668 669 670 671 672 673 674 675 676 677 678 679 680 681 682 683 684 685 686 687 688 689 690 691 692 693 694 695 696 697 698 699 700 701 702 703 704 705 706 707 708 709 710 711 712 713 714 715 716 717 718 719 720 721 722 723 724 725 726 727 728 729 730 731 732 733 734 735 736 737 738 739 740 741 742 743 744 745 746 747 748 749 750 751 752 753 754 755 756 757 758 759 760 761 762 763 764 765 766 767 768 769 770 771 772 773 774 775 776 777 778 779 780 781 782 783 784 785 786 787 788 789 790 791 792 793 794 795 796 797 798 799 800 801 802 803 804 805 806 807 808 809 810 811 812 813 814 815 816 817 818 819 820 821 822 823 824 825 826 827 828 829 830 831 832 833 834 835 836 837 838 839 840 841 842 843 844 845 846 847 848 849 850 851 852 853 854 855 856 857 858 859 860 861 862 863 864 865 866 867 868 869 870 871 872 873 874 875 876 875 878 879 880 881 882 883 884 885 886 887 888 889 890 891 892 893 894 895 896 897 898 899 900 901 902 903 904 905 906 907 908 909 910 911 912 913 914 915 916 917 918 919 920 921 922 923 924 925 926 927 928 929 930 931 932 933 934 935 936 937 938 939 940 941 942 943 944 945 946 947 948 949 950 951 952 953 954 955 956 957 958 959 960 961 962 963 964 965 966 967 968 969 970 971 972 973 974 975 976 977 978 979 980 981 982 983 984 985 986 987 988 989 990 991 992 993 994 995 996 997 998 999 1000 1001 1002 1003 1004 1005 1006 1007 1008 1009 1010 1011 1012 1013 1014 1015 1016 1017 1018 1019 1020 1021 1022 1023";

static const bool isDataSetValid3 = false;
static const std::string dataSet3 =
        "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 17 16 18 19 20 21 22 23 24 25 26 27 28 29 30 31";

// Node definition taken fromHackerRank; I added the m_index field for testing purposes.
struct Node
{
    Node( int data, int index ) :
            data( data ), m_index( index ), left( 0 ), right( 0 )
    {
    }
    ;
    int data;       // Node value
    int m_index;    // Node value that the node was inserted with.
    Node* left;
    Node* right;
};

class BST
{
public:
    BST() = default;
    BST( Node* root ) :
            m_root( root )
    {
    }
    BST( const BST& bst ) = delete;     // no copy constructor
    virtual ~BST();

    void run( std::string& dataSet );
    bool isValidTree()
    {
        return m_isValid;
    }
    bool isValidInput()
    {
        return m_inputInvalid;
    }

    bool checkBST();
    void printInputVector();
    void printTree();

private:
    // functions for creating tree
    void createTree();
    bool insertNodeByIndexValue( Node* root, Node*mnode );
    bool insertListIntoTree( unsigned int start_index, unsigned int end_index );

    //functions for checking input data
    void convert();
    void checkInputVector();

    // functions for checking tree.
    bool checkNode( Node* node, Node *root );
    Node* findNode( int data, Node* node );
    int getHighest( Node *node, int highestSoFar );
    int getLowest( Node *node, int lowestSoFar );
    // functions for printing data

    void printNode( Node* node, bool printChild );

    // functions for deleting tree
    void deleteNode( Node *node );

    // data
    Node* m_root = nullptr;       // tree root
    std::string m_ds;             // input data as a string of integers.
    std::vector<int> m_vec;       // input data as a vector of integers
    bool m_inputInvalid = false;  // indicates if data is valid
    bool m_isValid = false;       // indicates if tree is valid

    static const int maxVal = 10000;     // max value of data.
    static const bool debug = 1;
};

// function added to run in this test suite.
void validatekBinaryTree()
{
    T_START

    std::cout << "Validate that a tree is a valid binary tree \n"
         << "  1. The  value of every node in a node's left subtree is less than the data value of that node.\n"
         << "  2. The  value of every node in a node's right subtree is greater than the data value of that node."
         << std::endl;
    std::vector<std::string> setOfDataSets = { dataSet0, dataSet1, dataSet2, dataSet3 };
    std::vector<bool> setOfDataSetsValid = { isDataSetValid0, isDataSetValid1, isDataSetValid2,
            isDataSetValid3 };
    for( unsigned int i = 0; i < setOfDataSets.size(); i++ )
    {
        BST* bst = new BST();
        bst->run( setOfDataSets[i] );
        if( bst->isValidTree() != setOfDataSetsValid[i] )
        {
            std::cout << "FAILED: invalid result=" << bst->isValidTree() << " for setOfDataSets["
                    << i << "]";
        } else
        {
            std::cout << "SUCCESS: setOfDataSets[" << i << "]";
            std::cout << "Expected=" << setOfDataSetsValid[i] << " Actual=" << bst->isValidTree() << std::endl;
            if( bst->isValidTree() )
            {
                std::cout << "Tree is Valid" << std::endl;
            }
            else
            {
                std::cout << "Tree is NOT Valid" << std::endl;
            }
            std::cout << "\n" << std::endl;

        }


        delete bst;
    }
    T_END
}

bool BST::checkBST()
{
    assert( m_root );
    m_isValid = checkNode( m_root, m_root );
    return m_isValid;
}

// function I added to hackerrank site
// This checks to make sure that the tree is correct when the tree is evalueated from a data point of view.
// Note that tree insertion inserts by the value of the node index, not the value of the data.
// If the data is not equal to the index, the tree could be incorrect.e.g.
//     The  value of every node in a node's left subtree is less than the data value of that node.
//     The  value of every node in a node's right subtree is greater than the data value of that node.
bool BST::checkNode( Node* node, Node *root )
{
    T_START
    assert( node );
    assert( root );

    if( node->data > 10000 )
    {
        // constraint put on by HackerRankl.
        if( debug )
        {
            std::cout << "\n data too large > 10000 ";
            printNode( node, false );
            std::cout << std::endl;
        }
        return false;
    }

    if( node->left )
    {
        if( node->data <= node->left->data )
        {
            if( debug )
            {
                std::cout << "\ndata <= left data ";
                printNode( node, false );
                printNode( node->left, false );
                std::cout << std::endl;
            }
            return false;
        }
        if( ! checkNode( node->left, root ) )
        {
            if( debug )
            {
                std::cout << "\ncheck left against root failed";
                printNode( node->left, false );
                std::cout << std::endl;
            }
            return false;
        }
    }
    if( node->right )
    {
        if( node->data >= node->right->data )
        {
            if( debug )
            {
                std::cout << "\ndata >=right data ";
                printNode( node, false );
                printNode( node->right, false );
                std::cout << std::endl;
            }
            return false;
        }
        if( ! checkNode( node->right, root ) )
        {
            if( debug )
            {
                std::cout << "\ncheck right against root failed";
                printNode( node->right, false );
                std::cout << std::endl;
            }
            if( debug )

            return false;
        }
    }

    // check for duplicates; see if this node exists elsewhere in the tree
    Node* other = findNode( node->data, root );

    if( other && (other != node) )
    {
        // this node is a duplicate in the wrong spot
        if( debug )
        {
            std::cout << "\nnode exists twice in the tree:" << std::endl;
            std::cout << "other:[" << other->m_index << "]=" << other->data << std::endl;
            std::cout << "node:[" << node->m_index << "]=" << node->data << std::endl;
        }
        return false;
    }

    // check to make sure that all data to the left is lower than the data in this node
    int highestToLeft = getHighest( node->left, node->data );
    if( highestToLeft > node->data )
    {
        if( debug )
        {
            std::cout << "\ndata to left is is higher than node data ";
            printNode( node, false );
            std::cout << " highesttoLeft=" << highestToLeft << std::endl;
        }
        return false;
    }
    // check to make sure that all data to the right is higher than the data in this node
    int lowestToRight = getLowest( node->right, node->data );
    if( lowestToRight < node->data )
    {
        if( debug )
        {
            std::cout << "\ndata to right is is lower than node data ";
            printNode( node, false );
            std::cout << " lowestToRight=" << lowestToRight << std::endl;
        }
        return false;
    }
    T_END
    return true;
}

Node*
BST::findNode( int data, Node* node )
{
    if( ! node ) return 0;
    if( data == node->data ) return node;
    if( data < node->data )
        return (findNode( data, node->left ));
    else return (findNode( data, node->right ));
}

// get highest node data value number on or below this node
int BST::getHighest( Node *node, int highestSoFar )
{
    // int highest = highestSoFar;
    if( ! node ) return highestSoFar;
    if( node->data > highestSoFar ) highestSoFar = node->data;
    highestSoFar = getHighest( node->left, highestSoFar );
    highestSoFar = getHighest( node->right, highestSoFar );
    return highestSoFar;
}

// get lowest node data value on or below this node
int BST::getLowest( Node *node, int lowestSoFar )
{
    if( ! node ) return lowestSoFar;
    if( node->data < lowestSoFar ) lowestSoFar = node->data;
    lowestSoFar = getLowest( node->left, lowestSoFar );
    lowestSoFar = getLowest( node->right, lowestSoFar );
    return lowestSoFar;
}

BST::~BST()
{
    Node*node = m_root;
    delete (node);
}

void BST::deleteNode( Node *node )
{
    if( node )
    {
        deleteNode( node->left );
        deleteNode( node->right );
        delete node;
    }
}

void BST::run( std::string& dataSet )
{
    T_START
    m_ds = dataSet;
    convert();
    printInputVector();
    BST::checkInputVector();
    createTree();
    printTree();
    checkBST();
    T_END
}

// convert string dataset to a vector of integers
void BST::convert()
{
    T_START
    std::stringstream ss;
    ss << m_ds;
    std::string temp;
    while( ss >> temp )
    {
        try
        {
            int i = std::stoi( temp );
            assert( i < this->maxVal );  // max val mandataed by hackerRank
            m_vec.push_back( i );
        }
        catch( std::exception& e )
        {
            std::cout << "Invalid argument" << e.what() << std::endl;
            m_inputInvalid = false;
            return;
        }
    }
    m_inputInvalid = true;
    T_END
}

void BST::printInputVector()
{
    std::cout << "m_vec=" << std::endl;

    for( unsigned index = 0; index < m_vec.size(); index++ )
    {
        std::cout << " [" << index << "]=" << m_vec[index];
    }
    std::cout << "\nm_vec end" << std::endl;
}

// check inut vector to make sure that all Node values match the Node index value
void BST::checkInputVector()
{
    std::cout << "checking m_vec..." << std::endl;
    for( unsigned index = 0; index < m_vec.size(); index++ )
    {
        if( (int) (index + 1) != m_vec[index] )
        {
            std::cout << " [" << index << "]=" << m_vec[index];
        }
    }
    std::cout << "\nchecking m_vec end  ..." << std::endl;
}

// Insert vector dataset into a binary tree.  Use the index of the vector as the initial insert value, then change the
// value to match the input data.
void BST::createTree()
{
    T_START
    unsigned int size = m_vec.size();
    if( size == 0 )
    {
        std::cout << "cannot create tree; no input data" << std::endl;
        return;
    }
    unsigned int rootIndex = size / 2;
    unsigned int rootValue = m_vec[rootIndex];
    m_root = new Node( rootValue, rootIndex );
    if( size > 1 )
    {
        insertListIntoTree( 0, rootIndex - 1 );
    }
    if( size > 2 )
    {
        insertListIntoTree( rootIndex + 1, size - 1 );
    }
    T_END
}

bool BST::insertListIntoTree( unsigned int startIndex, unsigned int endIndex )
{
    T_START
    assert( endIndex >= startIndex );
    if( endIndex == startIndex )
    {
        insertNodeByIndexValue( m_root, new Node( m_vec[startIndex], startIndex ) );
    } else
    {
        // insert middle
        unsigned int middleIndex = (startIndex + endIndex) / 2;
        insertNodeByIndexValue( m_root, new Node( m_vec[middleIndex], middleIndex ) );
        // insert left range, then right range
        insertListIntoTree( startIndex, middleIndex - 1 );
        insertListIntoTree( middleIndex + 1, endIndex );
    }
    T_END
    return true;
}

bool BST::insertNodeByIndexValue( Node* root, Node* node )
{
    T_START
    assert( root );
    assert( node );
    bool retVal = true;
    if( root->m_index > node->m_index )
    {
        if( root->left )
            retVal = insertNodeByIndexValue( root->left, node );
        else root->left = node;
    }
    if( root->m_index < node->m_index )
    {
        if( root->right )
            retVal = insertNodeByIndexValue( root->right, node );
        else root->right = node;
    } else retVal = false;  // duplicate
    T_END
    return retVal;
}

void BST::printNode( Node* node, bool printChild )
{
    assert( node );
    if( printChild )
    {
        if( node->left ) printNode( node->left, printChild );
        std::cout << " [" << node->m_index << "]=" << node->data;
        if( node->right && printChild ) printNode( node->right, printChild );
    } else
    {
        std::cout << " [" << node->m_index << "]=" << node->data;
        if( node->left )
        {
            std::cout << " l[" << node->left->m_index << "]";
        } else
        {
            std::cout << " l[]";
        }
        if( node->right )
        {
            std::cout << " r[" << node->right->m_index << "]";
        } else
        {
            std::cout << " r[]";
        }
    }
    return;
}

void BST::printTree()
{
    T_START
    std::cout << "m_root=" << std::endl;
    if( m_root ) printNode( m_root, true );
    std::cout << std::endl;
    T_END;
}

// function provided on hackerrank site.
// define a Binary tree so we can test...
bool checkBST( Node* root )
{
    BST bst( root );
    return (bst.checkBST());
}

